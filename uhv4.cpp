#include "uhv4.h"
#include <ctime>
#include <cstdlib>
#include <random>
#include <iostream>
#include <set>


//Overload '<<' operator instead of using 'append' to accept larger parameter
/*QByteArray &operator <<(QByteArray &QBArr, const quint8 Dat)
{
    QBArr.append(Dat);
    return QBArr;
}

QByteArray &operator <<(QByteArray &QBArr, const quint16 Dat)
{
    return QBArr<<quint8(Dat>>8)<<quint8(Dat);
}

QByteArray &operator <<(QByteArray &QBArr, const quint32 Dat)
{
    return QBArr<<quint16(Dat>>16)<<quint16(Dat);
}

QByteArray &operator <<(QByteArray &QBArr, const quint64 Dat)
{
    return QBArr<<quint32(Dat>>32)<<quint32(Dat);
}

QByteArray &operator <<(QByteArray &QBArr, const QByteArray &Dat)
{
    QBArr.append(Dat);
    return QBArr;
}
*/
//Define the bases
static const QByteArray mModel ="abc"; //Model to be read
QByteArray tmp = "00000";
const UHV4::TypDATA UHV4::NumBase = tmp;

QByteArray tmp4 = "01";
const UHV4::TypWIN UHV4::HVBase = tmp4;

QByteArray tmp10 = "80";
const UHV4::TypWIN UHV4::TemperatureBase = tmp10;

QByteArray tmp11 = "0000000000";
const UHV4::TypDATA UHV4::ProtectBase = tmp11;


UHV4::UHV4(const quint8 Address)
    :mAddress(Address)
    ,msADDR(TypADDR(AddressBase + Address))
    ,msWIN("000")
    ,msCOM(COMRead)
    ,msDATA(* new TypDATA())
    ,mrDATA(* new TypDATA())
    ,msCRC("00")
    ,mRspMsg(* new TypMSG())
    ,mRcvMsg(* new TypMSG())
    ,mUnitPressure(* new QString("mBar"))
    ,mBaudrate(* new quint8(BaudRate9600))
    ,RangeLowV(* new int(0))
    ,RangeHighV(* new int (7000))
    ,RangeLowI(* new double(0.0000000001))
    ,RangeHighI(* new double(0.9))
{
    if (UHV4::UHV4ObjSet.contains(Address))   //If There's An Object With That BPNum, Then Throw Exception
        throw std::invalid_argument(std::string("Pump #")+std::to_string(Address)+std::string(" Has Already Existed !!!"));
    else
    {
        UHV4::UHV4ObjSet.insert(Address,this);
        for (int channel = 0; channel < 4; channel++)
        {
            mProtectMode[channel] = false;
            mTemperature[channel] = 0;
            mPressure[channel] = 0.0;
            mVoltage[channel] = 0;
            mCurrent[channel] = 0.0;
        }
        mTemperature[4] = 0;
    }
}

UHV4::~UHV4()
{
    UHV4::UHV4ObjSet.remove(mAddress);
}

UHV4 &UHV4::theUHV4(const quint8 Address)
{
    if (UHV4::UHV4ObjSet.contains(Address))
            return *UHV4::UHV4ObjSet.value(Address);
        else
            throw std::invalid_argument(std::string("Pump #")+std::to_string(Address)+std::string(" Does Not Exist !!!"));
}

bool UHV4::IsUHV4(const quint8 Address)
{
    return UHV4::UHV4ObjSet.contains(Address);
}

int UHV4::CountUHV4()
{
    return UHV4::UHV4ObjSet.size();
}

bool UHV4::DeleteUHV4(const quint8 Address)
{
    if(UHV4::UHV4ObjSet.contains(Address))
    {
        UHV4::UHV4ObjSet.value(Address)->~UHV4();
        return true;
    }
    else
        return false;
}

quint8 UHV4::GetUHV4Address() const
{
    return mAddress;
}

void UHV4::SetUHV4Address(const quint8 Address)
{
    mAddress = Address;
}

UHV4::TypADDR UHV4::GetADDR() const
{
    return mrADDR;
}

UHV4::TypWIN UHV4::GetWIN() const
{
    return mrWIN;
}

UHV4::TypCOM UHV4::GetCOM() const
{
    return mrCOM;
}

const UHV4::TypDATA &UHV4::GetDATA() const
{
    return mrDATA;
}

UHV4::TypCRC UHV4::GetCRC() const
{
    return mrCRC;
}



const UHV4::TypCRC UHV4::GenerateCRC(const QByteArray &data)
{
    // <STX> + <ADDR> + <WIN> + <COM> + <DATA> + <ETX>
    quint8 checksum = 0;
    QByteArray bytes[2];
    int len = data.size();
    //XOR all the bits AFTER STX
    for(int i = sizeof(STX); i < len; i++)
    {
        checksum = checksum ^ data[i];
    }
    //a check sum of a 2-digits hex
    //split it into two ASCII characters and convert to hex again
   UHV4::TypCRC CRC;
    CRC.append(QString::number((checksum /16),16).toUpper());
    CRC.append(QString::number((checksum %16),16).toUpper());

    return CRC;
}

const UHV4::TypMSG UHV4::GenerateMSG()
{
    mRspMsg.clear();
    UHV4::TypMSG rspmsg;
    rspmsg << STX << msADDR << msWIN << msCOM << msDATA << ETX;
    msCRC.clear();
    msCRC = UHV4::GenerateCRC(rspmsg);
    rspmsg << msCRC;

    msWIN = "000";
    msCOM = COMRead;
    msDATA.clear();
    msCRC = "00";

    return rspmsg;
}

bool UHV4::IsMatched(const TypADDR &ADDR)
{
    bool isMatched;
    if (ADDR == msADDR)
        isMatched = true;
    else
        isMatched = false;

    return isMatched;
}

void UHV4::HVSwitch(bool state, int channel)
{
    if (state)
        mHVState[channel - 1] = true;
    else
        mHVState[channel - 1] = false;

    qDebug() << "channel: " << channel << "on/off: " << mHVState[channel - 1] << endl;
}

UHV4 &UHV4::HVSwitch(int channel)
{
    msWIN.clear();
    msWIN = HVBase;


    switch (channel){
    case 1:
        msWIN << CHANNEL1;
        break;
    case 2:
        msWIN << CHANNEL2;
        break;
    case 3:
        msWIN << CHANNEL3;
        break;
    case 4:
        msWIN << CHANNEL4;
        break;
    }

    msCOM = COMRead;

    msDATA.clear();
    if (mHVState[channel - 1])
        msDATA << ChannelON;
    else
        msDATA << ChannelOFF;

    return *this;
}

void UHV4::UnitPressure(quint8 &unit)
{
    if (unit == PressureTorr)
        mUnitPressure = "Torr";
    else if (unit == PressuremBar)
        mUnitPressure = "mBar";
    else if (unit == PressurePa)
        mUnitPressure = "Pa";
}

UHV4 &UHV4::UnitPressure()
{
    msWIN.clear();
    msWIN = "600";
    msCOM = COMRead;
    msDATA.clear();
    msDATA = NumBase;
    if (mUnitPressure == "Torr")
        msDATA << PressureTorr;
    else if (mUnitPressure == "mBar")
        msDATA << PressuremBar;
    else if (mUnitPressure == "Pa")
        msDATA << PressurePa;

    return *this;
}

void UHV4::BaudRate(const quint8 &baudrate)
{
    std::set<quint8> BaudrateValues;
    BaudrateValues.insert(BaudRate1200);
    BaudrateValues.insert(BaudRate2400);
    BaudrateValues.insert(BaudRate4800);
    BaudrateValues.insert(BaudRate9600);

    if (BaudrateValues.find(baudrate) != BaudrateValues.end())
        mBaudrate = baudrate;
}

UHV4 &UHV4::BaudRate()
{
    msWIN.clear();
    msWIN = "108";
    msCOM = COMWrite;
    msDATA.clear();
    msDATA = NumBase;
    msDATA << mBaudrate;

    return *this;
}

/*void UHV4::ProtectMode(bool state, int channel)
{

}

UHV4 &UHV4::ProtectMode()
{

}*/

UHV4 &UHV4::Temperature(quint8 channel)
{
    msWIN.clear();
    msWIN = TemperatureBase;
    std::set<quint8> ChannelTempValues;
    ChannelTempValues.insert(CHANNELTEMP1);
    ChannelTempValues.insert(CHANNELTEMP2);
    ChannelTempValues.insert(CHANNELTEMP3);
    ChannelTempValues.insert(CHANNELTEMP4);
    ChannelTempValues.insert(FAN);

    if (ChannelTempValues.find(channel) != ChannelTempValues.end())
        msWIN << channel;
    else
    {
        //error
    }
    if (static_cast<int>(channel - '0') == 8)
        channel = '3';
    if (static_cast<int>(channel - '0') == 9)
        channel = '4';
    /*
    switch (channel){
    case 0:
        msWIN << FAN;
        break;
    case 1:
        msWIN << CHANNELTEMP1;
        break;
    case 2:
        msWIN << CHANNELTEMP2;
        break;
    case 3:
        msWIN << CHANNELTEMP3;
        break;
    case 4:
        msWIN << CHANNELTEMP4;
        break;
    }*/

    msCOM = COMRead;

    TypT t = RandomizeT();
    mTemperature[static_cast<int>(channel - '0')] = t;
    msDATA.clear();
    int d = t % 10;
    int c = ((t - d) % 100) / 10;
    int b = t / 100;

    msDATA = "000";
    msDATA << static_cast<quint8>(b + '0');
    msDATA << static_cast<quint8>(c + '0');
    msDATA << static_cast<quint8>(d + '0');

    return *this;
}

UHV4 &UHV4::Pressure(quint8 channel)
{
    msWIN.clear();
    msWIN << "8";

    std::set<quint8> ChannelValues;
    ChannelValues.insert(CHANNEL1);
    ChannelValues.insert(CHANNEL2);
    ChannelValues.insert(CHANNEL3);
    ChannelValues.insert(CHANNEL4);
    if (ChannelValues.find(channel) != ChannelValues.end())
        msWIN << channel;
    else
    {
        //error
    }
    msWIN << "2";

    msCOM = COMRead;

    TypV v = RandomizeV();
    qDebug () << "v " << v << endl;

    mVoltage[static_cast<int>(channel - '0') - 1] = v;

    TypP p = CalculateP(v);
    qDebug () << "p " << p << endl;
    mPressure[static_cast<int>(channel - '0') - 1] = p;

    msDATA.clear();

    //convert to h.nE-ab
    int a, b, h, n;
    int k = 0;
    while (p < 1)
    {
        p *= 10;
        k++;
    }
    h = p / 1;
    n = (p * 10) / 1;
    n = n % 10;

    a = k / 10;
    b = k % 10;

    msDATA << "000" << static_cast<quint8>(h + '0') << "." << static_cast<quint8>(n + '0') << "E" << "-" << static_cast<quint8>(a + '0') << static_cast<quint8>(b + '0');
    qDebug () << h << "." << n << "E" << "-" << a << b << endl;
    return *this;
}

UHV4 &UHV4::Current(quint8 channel)
{
    msWIN.clear();
    msWIN << "8";

    std::set<quint8> ChannelValues;
    ChannelValues.insert(CHANNEL1);
    ChannelValues.insert(CHANNEL2);
    ChannelValues.insert(CHANNEL3);
    ChannelValues.insert(CHANNEL4);
    if (ChannelValues.find(channel) != ChannelValues.end())
        msWIN << channel;
    else
    {
        //error
    }
    msWIN << "1";

    msCOM = COMRead;

    TypI i = RandomizeI();
    mCurrent[static_cast<int>(channel - '0') - 1] = i;

    msDATA.clear();

    //convert to hE-ab
    int a, b, h;
    int k = 0;
    while (i < 1)
    {
        i *= 10;
        k++;
    }
    h = i / 1;
    a = k / 10;
    b = k % 10;

    msDATA << NumBase << static_cast<quint8>(h + '0') << "E" << "-" << static_cast<quint8>(a + '0') << static_cast<quint8>(b + '0');
    qDebug () << h << "E-" << a << b << endl;

    return *this;
}



UHV4 &UHV4::Voltage(quint8 channel)
{
    msWIN.clear();
    msWIN << "8";

    std::set<quint8> ChannelValues;
    ChannelValues.insert(CHANNEL1);
    ChannelValues.insert(CHANNEL2);
    ChannelValues.insert(CHANNEL3);
    ChannelValues.insert(CHANNEL4);
    if (ChannelValues.find(channel) != ChannelValues.end())
        msWIN << channel;
    else
    {
        //error
    }

    /*switch (channel){
    case 1:
        msWIN << CHANNEL1;
        break;
    case 2:
        msWIN << CHANNEL2;
        break;
    case 3:
        msWIN << CHANNEL3;
        break;
    case 4:
        msWIN << CHANNEL4;
        break;
    }*/
    msWIN << "0";

    msCOM = COMRead;

    TypV v = RandomizeV();
    mVoltage[static_cast<int>(channel - '0') - 1] = v;

    msDATA.clear();
    msDATA << "0";
    int b = v / 10000;
    int c = (v - b * 10000) / 1000;
    int d = (v - b * 10000 - c * 1000) / 100;
    msDATA << static_cast<quint8>(b + '0');
    msDATA << static_cast<quint8>(c + '0');
    msDATA << static_cast<quint8>(d + '0');
    msDATA << "00";

    qDebug() << b << c << d << "00" << endl;
    return *this;
}

UHV4 &UHV4::Model()
{
    msWIN.clear();
    msWIN = "319";
    msCOM = COMRead;
    msDATA.clear();
    msDATA = mModel;

    return *this;
}

/*UHV4 &UHV4::Error()
{

}*/

//Values Generators
UHV4::TypV UHV4::RandomizeV()
{
    srand(time(NULL));
    return (rand() % (RangeHighV / 100) + (RangeLowV / 100)) * 100; //randomized between 0 and 10000
}

UHV4::TypI UHV4::RandomizeI()
{
    //Type of random number distribution
    std::uniform_real_distribution<double> dist(RangeLowI, RangeHighI);  //(min, max)
    //Mersenne Twister: Good quality random number generator
    std::mt19937 rng;
    //Initialize with non-deterministic seeds
    rng.seed(std::random_device{}());

    return dist(rng);
}

UHV4::TypP UHV4::CalculateP(TypV &voltage)
{
    UHV4::TypP pressure;
    //pressure = pow(10,(static_cast<double>(voltage) - 9.072)/0.833);
    //pressure = 500000.5*voltage - 0.000005;
    pressure = 6*pow(10, 0.221*(voltage - 4) - 7);

    qDebug() << "pressure: " << pressure << endl;
    return pressure;
}

UHV4::TypT UHV4::RandomizeT()
{
    srand(time(NULL));
    return rand() % 200 + 0; //randomized between 0 and 10000
}

void UHV4::PrepareMessage() //second step
{
    if(mrDATA.isEmpty() == false)
    {
        if (mrWIN.left(2) == "01")
        {
            bool state;
            if (*mrDATA.right(1) == ChannelON)
                state = true;
            else if (*mrDATA.right(1) == ChannelOFF)
                state = false;

            switch (mrWIN[2]){
            case CHANNEL1:
                HVSwitch(state, 1);
                break;
            case CHANNEL2:
                HVSwitch(state, 2);
                break;
            case CHANNEL3:
                HVSwitch(state,3);
                break;
            case CHANNEL4:
                HVSwitch(state, 4);
                break;
            }
        }
        else if (mrWIN == "108")
        {
            quint8 baudrate = *mrDATA.right(1);
            BaudRate(baudrate);
        }
        else if (mrWIN == "600")
        {
            quint8 unit = *mrDATA.right(1);
            UnitPressure(unit);
        }
       Acknowledge();
    }
    else
    {
        if (mrWIN.left(2) == "80" )
        {
            /*switch (mrWIN[2]){
            case FAN:
                mRspMsg = Temperature(0).GenerateMSG();
                break;
            case CHANNELTEMP1:
                mRspMsg = Temperature(1).GenerateMSG();
                break;
            case CHANNELTEMP2:
                mRspMsg = Temperature(2).GenerateMSG();
                break;
            case CHANNELTEMP3:
                mRspMsg = Temperature(3).GenerateMSG();
                break;
            case CHANNELTEMP4:
                mRspMsg = Temperature(4).GenerateMSG();
                break;
            }*/
            mRspMsg = Temperature(mrWIN[2]).GenerateMSG();
        }
        else if (mrWIN[0] == '8')
        {
            if (mrWIN[2] == '0')
                mRspMsg = Voltage(mrWIN[1]).GenerateMSG();
            else if (mrWIN[2] == '1')
                mRspMsg = Current(mrWIN[1]).GenerateMSG();
            else if (mrWIN[2] == '2')
                mRspMsg = Pressure(mrWIN[1]).GenerateMSG();
        }

    }
    SendMessage(mRspMsg);
}

void UHV4::SplitRcvMsgComponents(QByteArray rMsg) //first step
{
    quint8 tmp1 = sizeof(STX);
    UHV4 &UHV4Tmp = UHV4::theUHV4(mAddress);
    UHV4Tmp.mRcvMsg.clear();
    UHV4Tmp.mRcvMsg = rMsg;

    UHV4Tmp.mrADDR = *rMsg.mid(sizeof(STX), SzADDR);
    tmp1 += SzADDR;
    UHV4Tmp.mrWIN = rMsg.mid(tmp1, SzWIN);
    tmp1 += SzWIN;
    UHV4Tmp.mrCOM = *rMsg.mid(tmp1, SzCOM);
    tmp1 += SzCOM;
    quint8 tmp2 = rMsg.size();
    UHV4Tmp.mrCRC = rMsg.right(SzCRC);
    tmp2 -= tmp1 + SzCRC + sizeof(ETX);
    UHV4Tmp.mrDATA = rMsg.mid(tmp1,tmp2);
    qDebug() << "CRC" << GenerateCRC(rMsg.left(rMsg.size() - 2)).toHex() << endl;
    qDebug() << "mCRC" << UHV4Tmp.mrCRC.toHex() << endl;
    if (GenerateCRC(rMsg.left(rMsg.size() - 2)) == UHV4Tmp.mrCRC)
    {
        qDebug() << "chinh xac" << endl;
        qDebug() << "message la" << rMsg.toHex() << endl;
        qDebug() << "adress: " << QString::number(mrADDR,16) << endl << "win: " << mrWIN.toHex() << endl << "com: " << QString::number(mrCOM,16) << endl <<  "data" << mrDATA.toHex() << endl << "crc: " << mrCRC.toHex() << endl;
        //Next step
        PrepareMessage();
    }
    else
        qDebug() << "sai roi ku" << endl;
}

void UHV4::SetRangeV(int high, int low)
{
    RangeHighV = high;    
    RangeLowV = low;
    qDebug() << "UHV4: " << mAddress << endl;
    qDebug() << "low V: " << RangeLowV << endl;
    qDebug() << "high V: " << RangeHighV << endl;

}

void UHV4::SetRangeI(double high, double low)
{
    RangeHighI = high;
    RangeLowI = low;
    qDebug() << "UHV4: " << mAddress << endl;
    qDebug() << "low I: " << RangeLowI << endl;
    qDebug() << "high I: " << RangeHighI << endl;    
}

void UHV4::SetSerialInterface(QString portname)
{
    if (SerialInterface::IsSI(portname))
        SI = &SerialInterface::SI(portname);
    else
        SI = new SerialInterface(portname);
    qDebug() << "serial la: " << SI->GetPortName() << endl;
}

void UHV4::SendMessage(const TypMSG &sMsg)
{
    SI->simpleWriteNow(sMsg);
}

void UHV4::Acknowledge()
{
    mRspMsg = QByteArray::fromHex("028006033835");
}

QHash<const quint8, UHV4 *> &UHV4::UHV4ObjSet = * new QHash<const quint8, UHV4 *>();

