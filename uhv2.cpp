#include "uhv2.h"
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
}*/
//<hdr> <length> <command> <channel> <data> <checksum>
UHV2::UHV2(const quint8 Address)
    :mAddress(Address)
    ,msHdr(TypHdr(HdrRspBase + Address))
    ,msLn("00")
    ,msCmd("00")
    ,msCh(CHANNEL0)
    ,msDt(* new TypDt())
    ,mrDt(* new TypDt())
    ,msCksm('0')
    ,mRcvMsg(* new TypMsg())
    ,mRspMsg(* new TypMsg())
    ,RangeLowV(* new int(0))
    ,RangeHighV(* new int (7000))
    ,RangeLowI(* new double(0.0000000001))
    ,RangeHighI(* new double(0.9))
{
    if (UHV2::UHV2ObjSet.contains(Address))   //If There's An Object With That BPNum, Then Throw Exception
        throw std::invalid_argument(std::string("Pump #")+std::to_string(Address)+std::string(" Has Already Existed !!!"));
    else
    {
        UHV2::UHV2ObjSet.insert(Address,this);
        for (int channel = 0; channel < 2; channel++)
        {
            mPressure[channel] = 0.0;
            mVoltage[channel] = 0;
            mCurrent[channel] = 0.0;
        }
    }
}

UHV2::~UHV2()
{
    UHV2::UHV2ObjSet.remove(mAddress);
}

UHV2 &UHV2::theUHV2(const quint8 Address)
{
    if (UHV2::UHV2ObjSet.contains(Address))
            return *UHV2::UHV2ObjSet.value(Address);
        else
            throw std::invalid_argument(std::string("Pump #")+std::to_string(Address)+std::string(" Does Not Exist !!!"));
}

bool UHV2::IsUHV2(const quint8 Address)
{
    return UHV2::UHV2ObjSet.contains(Address);
}

int UHV2::CountUHV2()
{
    return UHV2::UHV2ObjSet.size();
}

bool UHV2::DeleteUHV2(const quint8 Address)
{
    if(UHV2::UHV2ObjSet.contains(Address))
    {
        UHV2::UHV2ObjSet.value(Address)->~UHV2();
        return true;
    }
    else
        return false;
}

quint8 UHV2::GetUHV2Address() const
{
    return mAddress;
}

void UHV2::SetUHV2Address(const quint8 Address)
{
    mAddress = Address;
}

const UHV2::TypCksm UHV2::GenerateCksm(const QByteArray &data)
{
  quint8 checksum = 0;
  int len = data.size();

  for(int i = 0; i< len; i++)
  {
      checksum ^= data[i];
  }
  checksum ^= 0x7F; //XOR with 7F

  return checksum;
}

const UHV2::TypMsg UHV2::GenerateMsg()
{
  mRspMsg.clear();
  UHV2::TypMsg rspmsg;
  //<hdr> <length> <command> <channel> <data> <checksum>
  rspmsg << msHdr << msLn << msCmd << msCh;
  msCksm = UHV2::GenerateCksm(rspmsg);
  rspmsg << msCksm;

  msLn.clear();
  msCmd.clear();
  msDt.clear();
  msCksm = 0;

  return rspmsg;
}

void UHV2::HVSwitch(bool state, int channel)
{
    if (state)
        mHVState[channel - 1] = true;
    else
        mHVState[channel - 1] = false;

    qDebug() << "channel: " << channel << "on/off: " << mHVState[channel - 1] << endl;
}

UHV2 &UHV2::HVSwitch(quint8 channel)
{
    msLn << "04";

    msCmd << QByteArray::fromHex("4130");

    msDt.clear();
    if (channel == CHANNEL1)
    {
        msCh = channel;
        if (mHVState[0])
            msDt << ChannelON;
        else
            msDt << ChannelOFF;
    }else if (channel == CHANNEL2)
    {
        msCh = channel;
        if (mHVState[1])
            msDt << ChannelON;
        else
            msDt << ChannelOFF;
    }

    return *this;
}

UHV2 &UHV2::Voltage(quint8 channel)
{
    msLn << "08";

    msCmd << QByteArray::fromHex("5330");

    TypV v = RandomizeV();

    if (channel == CHANNEL1)
    {
        msCh = channel;
        mVoltage[0] = v;
    }else if (channel == CHANNEL2)
    {
        msCh = channel;
        mVoltage[1] = v;
    }

    msDt.clear();
    int b = v / 10000;
    int c = (v - b * 10000) / 1000;
    int d = (v - b * 10000 - c * 1000) / 100;
    msDt << static_cast<quint8>(b + '0');
    msDt << static_cast<quint8>(c + '0');
    msDt << static_cast<quint8>(d + '0');
    msDt << "00";

    return *this;
}

UHV2 &UHV2::Current(quint8 channel)
{
    msLn << "10";

    msCmd << QByteArray::fromHex("5430");

    TypI i = RandomizeI();

    if (channel == CHANNEL1)
    {
        msCh = channel;
        mCurrent[0] = i;
    } else if (channel == CHANNEL2)
    {
        msCh = channel;
        mCurrent[1] = i;
    }

    msDt.clear();

    int a, b, h, n;
    int k = 0;
    while (i < 1)
    {
        i *= 10;
        k++;
    }
    h = i / 1;
    n = (i * 10) / 1;
    n = n % 10;

    a = k / 10;
    b = k % 10;

    msDt << static_cast<quint8>(h + '0') << " " << static_cast<quint8>(n + '0') << "E" << "-" << static_cast<quint8>(a + '0') << static_cast<quint8>(b + '0');

    return *this;
}

UHV2 &UHV2::Pressure(quint8 channel)
{
    msLn << "10";

    msCmd << QByteArray::fromHex("5530");

    TypV v = RandomizeV();
    TypP p = CalculateP(v);

    if (channel == CHANNEL1)
    {
        msCh = channel;
        mVoltage[0] = v;
        mPressure[0] = p;
    } else if (channel == CHANNEL2)
    {
        msCh = channel;
        mVoltage[1] = v;
        mPressure[1] = p;
    }

    msDt.clear();

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

    msDt << static_cast<quint8>(h + '0') << " " << static_cast<quint8>(n + '0') << "E" << "-" << static_cast<quint8>(a + '0') << static_cast<quint8>(b + '0');

    return *this;
}

UHV2::TypV UHV2::RandomizeV()
{
    srand(time(NULL));
    return (rand() % (RangeHighV / 100) + (RangeLowV / 100)) * 100; //randomized between 0 and 10000
}

UHV2::TypI UHV2::RandomizeI()
{
    //Type of random number distribution
    std::uniform_real_distribution<double> dist(RangeLowI, RangeHighI);  //(min, max)
    //Mersenne Twister: Good quality random number generator
    std::mt19937 rng;
    //Initialize with non-deterministic seeds
    rng.seed(std::random_device{}());

    return dist(rng);
}

UHV2::TypP UHV2::CalculateP(TypV &voltage)
{
    UHV2::TypP pressure;
    //pressure = pow(10,(static_cast<double>(voltage) - 9.072)/0.833);
    //pressure = 500000.5*voltage - 0.000005;
    pressure = 6*pow(10, 0.221*(voltage - 4) - 7);

    qDebug() << "pressure: " << pressure << endl;
    return pressure;
}

void UHV2::PrepareMessage() //understand the command and set the components for the respond message
{
    if (*mrDt.right(1) != READ)
    {
        if (mrCmd == QByteArray::fromHex("4130")) //HV switch
        {
            bool state;
            if (*mrDt.right(1) == ChannelON)
                state = true;
            else if (*mrDt.right(1) == ChannelOFF)
                state = false;
            if (mrCh == CHANNEL1)
                HVSwitch(state, 1);
            else if (mrCh == CHANNEL2)
                HVSwitch(state, 2);
        }
        Acknowledge();
    } else if (mrCh == CHANNEL1 || mrCh == CHANNEL2)
    {
        if (mrCmd == QByteArray::fromHex("4130")) //HV Switch
            mRspMsg = HVSwitch(mrCh).GenerateMsg();
        else if (mrCmd == QByteArray::fromHex("5330")) //Voltage
            mRspMsg = Voltage(mrCh).GenerateMsg();
        else if (mrCmd == QByteArray::fromHex("5430")) //Current
            mRspMsg = Current(mrCh).GenerateMsg();
        else if (mrCmd == QByteArray::fromHex("5530")) //Pressure
            mRspMsg = Pressure(mrCh).GenerateMsg();
    }

    SendMessage(mRspMsg);
}

void UHV2::SplitRcvMsgComponents(QByteArray rMsg) //split -> prepare -> send
{
    UHV2 &UHV2Tmp = UHV2::theUHV2(mAddress);
    UHV2Tmp.mRcvMsg.clear();
    UHV2Tmp.mRcvMsg = rMsg;
    //<hdr> <length> <command> <channel> <data> <checksum>

    quint8 tmp1 = SzHdr;
    UHV2Tmp.mrHdr = *rMsg.left(SzHdr);
    UHV2Tmp.mrLn = rMsg.mid(tmp1, SzLn);
    tmp1 += SzLn;
    UHV2Tmp.mrCmd = rMsg.mid(tmp1,SzCmd);
    tmp1 += SzCmd;
    UHV2Tmp.mrCh = *rMsg.mid(tmp1, SzCh);
    tmp1 += SzCh;
    quint8 tmp2 = rMsg.size();
    UHV2Tmp.mrCksm = *rMsg.right(SzCksm);
    UHV2Tmp.msDt = rMsg.mid(tmp1, tmp2);

    if (GenerateCksm(rMsg.left(rMsg.size() - SzCksm)) == UHV2Tmp.mrCksm)
    {
        PrepareMessage();
    }
}

void UHV2::SetRangeV(int high, int low)
{
    RangeHighV = high;
    RangeLowV = low;
}

void UHV2::SetRangeI(double high, double low)
{
    RangeHighI = high;
    RangeLowI = low;
}

void UHV2::SetSerialInterface(QString portname)
{
    if (SerialInterface::IsSI(portname))
        SI = &SerialInterface::SI(portname);
    else
        SI = new SerialInterface(portname);
    qDebug() << "serial la: " << SI->GetPortName() << endl;
}

void UHV2::SendMessage(const TypMsg &sMsg)
{
    SI->simpleWriteNow(sMsg);
}

void UHV2::Acknowledge()
{
    //Check the rsp of UHV2 (ACK)
    mRspMsg = QByteArray::fromHex("06");
}

QHash<const quint8, UHV2 *> &UHV2::UHV2ObjSet = * new QHash<const quint8, UHV2 *>();
