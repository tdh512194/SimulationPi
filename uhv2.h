#ifndef UHV2_H
#define UHV2_H

#include <QHash>
#include <set>
#include <QObject>
#include <serialinterface.h>
#include <QDebug>
#include "straystuffs.h"

class UHV2: public QObject
{
    
    Q_OBJECT
public:
    
    ///Define class types
    
    //Measured values types
    typedef double TypP;
    typedef double TypI;
    typedef int TypV;
    
    typedef QByteArray TypMsg;
    //Message components types
    typedef quint8 TypHdr;
    typedef QByteArray TypLn;
    typedef QByteArray TypCmd;
    typedef quint8 TypCh;
    typedef QByteArray TypDt;
    typedef quint8 TypCksm;
    
    ///Define class constants
    
    //Fixed size in byte of message components
    static const quint8 SzHdr = 1;
    static const quint8 SzLn = 2;
    static const quint8 SzCmd = 2;
    static const quint8 SzCh = 1;
    static const quint8 SzCksm = 1;
    
    //Fixed values for message components
    static const quint8 HdrCmdBase = 0x81;
    static const quint8 HdrRspBase = 0x01;

    static const quint8 CHANNEL0 = 0x30;
    static const quint8 CHANNEL1 = 0x31;
    static const quint8 CHANNEL2 = 0x32;

    static const quint8 ChannelOFF = 0x30;
    static const quint8 ChannelON = 0x31;
    static const quint8 READ = 0x3F;
    
private:

    static QHash <const quint8, UHV2 *> &UHV2ObjSet;

    quint8 mAddress;

    bool HVState[2];
    bool * const &mHVState = HVState;

    TypP tPressure[2];
    TypP * const &mPressure = tPressure;

    TypI tCurrent[2];
    TypI * const &mCurrent = tCurrent;

    TypV tVoltage[2];
    TypV * const &mVoltage = tVoltage;
    
    int &RangeHighV;
    int &RangeLowV;
    double &RangeHighI;
    double &RangeLowI;

    //
    TypMsg &mRcvMsg;
    TypMsg &mRspMsg;

    //Msg components
    //<hdr> <length> <command> <channel> <data> <checksum>
    TypHdr mrHdr;
    TypLn mrLn;
    TypCmd mrCmd;
    TypCh mrCh;
    TypDt &mrDt;
    TypCksm mrCksm;

    TypHdr msHdr;
    TypLn msLn;
    TypCmd msCmd;
    TypCh msCh;
    TypDt &msDt;
    TypCksm msCksm;

    SerialInterface *SI;

public:
    explicit UHV2(const quint8 Address);
    ~UHV2();
    static UHV2 &theUHV2(const quint8 Address);
    static bool IsUHV2(const quint8 Address);
    static int CountUHV2();
    static bool DeleteUHV2(const quint8 Address);

    quint8 GetUHV2Address() const;
    void SetUHV2Address(const quint8 Address);

    //Get Messages Components
    TypHdr GetHdr() const;
    TypLn GetLn() const;
    TypCmd GetCmd() const;
    TypCh GetCh() const;
    const TypDt GetDt() const;
    TypCksm GetCksm() const;

    const TypCksm GenerateCksm(const QByteArray &data);
    const TypMsg GenerateMsg();

    bool IsMatched(const quint8 &address);

    void HVSwitch(bool state, int channel);

    UHV2 &HVSwitch(quint8 channel);
    UHV2 &Pressure(quint8 channel);
    UHV2 &Current(quint8 channel);
    UHV2 &Voltage(quint8 channel);

    //Values Generators
    TypV RandomizeV();
    TypI RandomizeI();
    TypP CalculateP(TypV &voltage); //mBarr

    void Acknowledge();
    void PrepareMessage();
    void SendMessage(const TypMsg &sMsg);

public slots:

    void SetSerialInterface(QString portname);
    //Range modifier
    void SetRangeV(int high, int low);
    void SetRangeI(double high, double low);

    void SplitRcvMsgComponents(QByteArray rMsg);
};

#endif // UHV2_H
