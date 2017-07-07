#ifndef UHV4_H
#define UHV4_H

#include "straystuffs.h"
#include <QHash>
#include <set>
#include <QObject>
#include <serialinterface.h>
#include <QDebug>

class UHV4: public QObject
{

    Q_OBJECT
public:


    ///Define class types

    //Measured values types
    typedef double TypP;
    typedef double TypI;
    typedef int TypV;
    typedef int TypT;
    typedef quint8 TypADDR;
    //Message components types
    typedef QByteArray TypMSG;

    typedef QByteArray TypWIN;
    typedef quint8 TypCOM;
    typedef QByteArray TypDATA;
    typedef QByteArray TypCRC;

    ///Define class constants
    //Fixed Size in Byte of Message Components (need discussed)
    //Size of DATA is not constant
    static const quint8 SzADDR = 1;
    static const quint8 SzWIN = 3;
    static const quint8 SzCOM = 1;
    static const quint8 SzCRC = 2;

    //Fixed Values of Message Components
    static const quint8 STX = 0x02;
    static const quint8 ETX = 0x03;
    static const TypADDR AddressBase = 0x80;
    static const TypCOM COMRead = 0x30; //default
    static const TypCOM COMWrite = 0x31;
    static const quint8 CHANNEL1 = 0x31;
    static const quint8 CHANNEL2 = 0x32;
    static const quint8 CHANNEL3 = 0x33;
    static const quint8 CHANNEL4 = 0x34;

    static const TypDATA NumBase;
    static const TypDATA ProtectBase;

    //Fixed Values of DATA Components and Bases correspond with WIN
      /* WIN HV ON/OFF */

    static const quint8 ChannelON = 0x31;
    static const quint8 ChannelOFF = 0x30; //default

    static const TypWIN HVBase;// = {0x30, 0x31}; //append CHANNEL1 - 4 for Channel 1 - 4

     /* WIN Baud rate */
    static const quint8 BaudRate1200 = 0x31;
    static const quint8 BaudRate2400 = 0x32;
    static const quint8 BaudRate4800 = 0x33;
    static const quint8 BaudRate9600 = 0x34;

     /* WIN Unit Pressure */
    static const quint8 PressureTorr = 0x30;
    static const quint8 PressuremBar = 0x31; //default
    static const quint8 PressurePa = 0x32;

     /* WIN Temperature */
    static const TypWIN TemperatureBase;// = {0x38, 0x30}; //append FAN, CHANNELFAN1 - 4 for Channel 1 - 4
     //to append because this is an exception for channel 3 and 4

    static const quint8 FAN = 0x30;
    static const quint8 CHANNELTEMP1 = 0x31;
    static const quint8 CHANNELTEMP2 = 0x32;
    static const quint8 CHANNELTEMP3 = 0x38;
    static const quint8 CHANNELTEMP4 = 0x39;

     /* WIN Measurement */
    static const quint8 MeasuredBase = 0x38; //append CHANNEL1 - 4 then append MeasuredV / I / P
    static const quint8 MeasuredV = 0x30;
    static const quint8 MeasuredI = 0x31;
    static const quint8 MeasuredP = 0x32;

private:
    /// Hash tables

    //Class instances
    static QHash <const quint8, UHV4 *> &UHV4ObjSet;

    //Hash table for Some WINs
    static QHash <const TypWIN, QString> &WindowSet;

    ///Declare object Properties

    quint8 mAddress;

    bool HVState[4]; //HV ON/OFF array
    bool * const &mHVState = HVState; //reference to the HV ON/OFF array

    quint8 &mBaudrate; //Baudrate of RS485

    QString &mUnitPressure;

    bool tProtectMode[4]; //Protect mode array and its reference
    bool * const &mProtectMode = tProtectMode;

    TypT tTemperature[5]; //Temperature and its reference
    TypT * const &mTemperature = tTemperature;

    TypP tPressure[4]; //Pressure and its reference
    TypP * const &mPressure = tPressure;

    TypV tVoltage[4];
    TypV * const &mVoltage = tVoltage;

    TypI tCurrent[4];
    TypI * const &mCurrent = tCurrent;

    int &RangeHighV;
    int &RangeLowV;
    double &RangeHighI;
    double &RangeLowI;

    TypMSG &mRcvMsg;
    TypMSG &mRspMsg;

    //Message components
    TypADDR mrADDR;
    TypWIN mrWIN;
    TypCOM mrCOM;
    TypDATA &mrDATA;
    TypCRC mrCRC;

    TypADDR msADDR;
    TypWIN msWIN;
    TypCOM msCOM;
    TypDATA &msDATA;
    TypCRC msCRC;

    SerialInterface *SI;



public:
    explicit UHV4(const quint8 Address);
    ~UHV4();
    static UHV4 &theUHV4(const quint8 Address);
    static bool IsUHV4(const quint8 Address);
    static int CountUHV4();
    static bool DeleteUHV4(const quint8 Address);

    quint8 GetUHV4Address() const;
    void SetUHV4Address(const quint8 Address);

    //Get Messages Components
    TypADDR GetADDR() const;
    TypWIN GetWIN() const;
    TypCOM GetCOM() const;
    const TypDATA &GetDATA() const;
    TypCRC GetCRC() const;

    const TypCRC GenerateCRC(const QByteArray &data);
    const TypMSG GenerateMSG();

    bool IsMatched(const TypADDR &ADDR); //validate the address of the message with the UHV4

    void HVSwitch(bool state, int channel);
    void UnitPressure(quint8 &unit );
    void BaudRate(const quint8 &baudrate);
    void ProtectMode(bool state, int channel);

    UHV4 &HVSwitch(int channel);
    UHV4 &UnitPressure();
    UHV4 &BaudRate();
    UHV4 &ProtectMode();
    UHV4 &Temperature(quint8 channel);
    UHV4 &Pressure(quint8 channel);
    UHV4 &Current(quint8 channel);
    UHV4 &Voltage(quint8 channel);
    UHV4 &Model();
    UHV4 &Error();

    //Values Generators
    TypV RandomizeV();
    TypI RandomizeI();
    TypP CalculateP(TypV &voltage); //mBarr
    TypT RandomizeT();

    void Acknowledge();
    void PrepareMessage();
    void SendMessage(const TypMSG &sMsg);

public slots:

    void SetSerialInterface(QString portname);
    //Range modifier
    void SetRangeV(int high, int low);
    void SetRangeI(double high, double low);

    void SplitRcvMsgComponents(QByteArray rMsg);
};

#endif // UHV4_H
