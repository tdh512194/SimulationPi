#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //qRegisterMetaType<QByteArray&>("QbyteArray&");
}

MainWindow::~MainWindow()
{
    delete ui;
    InitMainWindow();
}

void MainWindow::InitMainWindow()
{
    const auto PortInfos = QSerialPortInfo::availablePorts();
    const quint8 int8tmp = PortInfos.size();
    foreach (const QSerialPortInfo &info, PortInfos)
    {
        ui->comboBox1->addItem(info.portName());
        ui->comboBox2->addItem(info.portName());
        ui->comboBox3->addItem(info.portName());
        ui->comboBox4->addItem(info.portName());
    }

}


void MainWindow::on_comboBox1_currentIndexChanged(int index)
{
    if (index == 0)
        ui->comboBox1->setItemText(0,"Select Interface");
    else
    {
        ui->comboBox1->setItemText(0,"Disconnect All");
        QString *StrTmp = new QString(ui->comboBox1->currentText());
        if (SerialInterface::IsSI(*StrTmp))
            NowSI1 = &SerialInterface::SI(*StrTmp);
        else
            NowSI1 = new SerialInterface(*StrTmp);
    }
}

void MainWindow::on_comboBox2_currentIndexChanged(int index)
{
    if (index == 0)
        ui->comboBox2->setItemText(0,"Select Interface");
    else
    {
        ui->comboBox2->setItemText(0,"Disconnect All");
        QString *StrTmp = new QString(ui->comboBox2->currentText());
        if (SerialInterface::IsSI(*StrTmp))
            NowSI2 = &SerialInterface::SI(*StrTmp);
        else
            NowSI2 = new SerialInterface(*StrTmp);
    }
}

void MainWindow::on_comboBox3_currentIndexChanged(int index)
{
    if (index == 0)
        ui->comboBox3->setItemText(0,"Select Interface");
    else
    {
        ui->comboBox3->setItemText(0,"Disconnect All");
        QString *StrTmp = new QString(ui->comboBox3->currentText());
        if (SerialInterface::IsSI(*StrTmp))
            NowSI3 = &SerialInterface::SI(*StrTmp);
        else
            NowSI3 = new SerialInterface(*StrTmp);
    }
}

void MainWindow::on_comboBox4_currentIndexChanged(int index)
{
    if (index == 0)
        ui->comboBox4->setItemText(0,"Select Interface");
    else
    {
        ui->comboBox4->setItemText(0,"Disconnect All");
        QString *StrTmp = new QString(ui->comboBox4->currentText());
        if (SerialInterface::IsSI(*StrTmp))
            NowSI4 = &SerialInterface::SI(*StrTmp);
        else
            NowSI4 = new SerialInterface(*StrTmp);
    }
}

void MainWindow::on_checkBox1_stateChanged(int state)
{
    QThread *thread_1;
    thread_1 = new QThread;

    if (ui->checkBox1->isChecked())
    {
        qDebug() << "check box checked" << endl;
        if (ui->radioButton12->isChecked())
        {
            qDebug() << "radio button checked" << endl;
            UHV4 *uhv4_1;
            uhv4_1 = new UHV4(UHV4::CountUHV4()+1);
            UHV4Tmp_1 = uhv4_1;
            qDebug() << "address la: " << QString::number(uhv4_1->GetUHV4Address(), 16) << endl;

            uhv4_1->moveToThread(thread_1);
            qDebug() << "Serial Interface: " << NowSI1->GetPortName() << endl;

            connect(this, SIGNAL(setSerialInterface(QString)), UHV4Tmp_1, SLOT(SetSerialInterface(QString)));
            emit setSerialInterface(NowSI1->mPortName); //set the serial interface for the object
            disconnect(this, SIGNAL(setSerialInterface(QString)), 0, 0);
            connect(this, SIGNAL(receiveMsg1(QByteArray)), UHV4Tmp_1, SLOT(SplitRcvMsgComponents(QByteArray)));
            connect(&NowSI1->mSerialPort, SIGNAL(readyRead()), this, SLOT(serialport14_read())); //emit receiveMsg signal
            //connect(uhv4_1, SIGNAL(SendMessage(QByteArray)), NowSI1, SLOT(simpleWriteNow(QByteArray)));
            //connect(thread_1, SIGNAL(started()), uhv4_1

            thread_1->start();
        }
        else if (ui->radioButton11->isChecked())
        {
            qDebug() << "radio button checked" << endl;
            UHV2 *uhv2_1;
            uhv2_1 = new UHV2(UHV2::CountUHV2()+1);
            UHV2Tmp_1 = uhv2_1;
            qDebug() << "address la: " << QString::number(uhv2_1->GetUHV2Address(), 16) << endl;

            uhv2_1->moveToThread(thread_1);
            qDebug() << "Serial Interface: " << NowSI1->GetPortName() << endl;

            connect(this, SIGNAL(setSerialInterface(QString)), UHV2Tmp_1, SLOT(SetSerialInterface(QString)));
            emit setSerialInterface(NowSI1->mPortName); //set the serial interface for the object
            disconnect(this, SIGNAL(setSerialInterface(QString)), 0, 0);
            connect(this, SIGNAL(receiveMsg1(QByteArray)), UHV2Tmp_1, SLOT(SplitRcvMsgComponents(QByteArray)));
            connect(&NowSI1->mSerialPort, SIGNAL(readyRead()), this, SLOT(serialport12_read())); //emit receiveMsg signal
            //connect(uhv4_1, SIGNAL(SendMessage(QByteArray)), NowSI1, SLOT(simpleWriteNow(QByteArray)));
            //connect(thread_1, SIGNAL(started()), uhv4_1

            thread_1->start();
        }
    }
    else if (ui->checkBox1->isChecked() == false)
    {
       thread_1->terminate();
    }
}

void MainWindow::on_checkBox2_stateChanged(int state)
{
    QThread *thread_2;
    thread_2 = new QThread;

    if (ui->checkBox2->isChecked())
    {
        if (ui->radioButton21->isChecked())
        {
            qDebug() << "radio button checked" << endl;
            UHV2 *uhv2_2;
            uhv2_2 = new UHV2(UHV2::CountUHV2()+1);
            UHV2Tmp_2 = uhv2_2;
            qDebug() << "address la: " << QString::number(uhv2_2->GetUHV2Address(), 16) << endl;

            uhv2_2->moveToThread(thread_2);
            qDebug() << "Serial Interface: " << NowSI2->GetPortName() << endl;

            connect(this, SIGNAL(setSerialInterface(QString)), UHV2Tmp_2, SLOT(SetSerialInterface(QString)));
            emit setSerialInterface(NowSI2->mPortName); //set the serial interface for the object
            disconnect(this, SIGNAL(setSerialInterface(QString)), 0, 0);
            connect(this, SIGNAL(receiveMsg2(QByteArray)), UHV2Tmp_2, SLOT(SplitRcvMsgComponents(QByteArray)));
            connect(&NowSI2->mSerialPort, SIGNAL(readyRead()), this, SLOT(serialport22_read())); //emit receiveMsg signal
            //connect(uhv4_1, SIGNAL(SendMessage(QByteArray)), NowSI1, SLOT(simpleWriteNow(QByteArray)));
            //connect(thread_1, SIGNAL(started()), uhv4_1

            thread_2->start();
        }
        else if (ui->radioButton22->isChecked())
        {
            qDebug() << "radio button checked" << endl;
            UHV4 *uhv4_2;
            uhv4_2 = new UHV4(UHV4::CountUHV4()+1);
            UHV4Tmp_2 = uhv4_2;
            qDebug() << "address la: " << QString::number(uhv4_2->GetUHV4Address(), 16) << endl;

            uhv4_2->moveToThread(thread_2);
            qDebug() << "Serial Interface: " << NowSI2->GetPortName() << endl;

            connect(this, SIGNAL(setSerialInterface(QString)), UHV4Tmp_2, SLOT(SetSerialInterface(QString)));
            emit setSerialInterface(NowSI2->mPortName); //set the serial interface for the object
            disconnect(this, SIGNAL(setSerialInterface(QString)), 0, 0);
            connect(this, SIGNAL(receiveMsg2(QByteArray)), UHV4Tmp_2, SLOT(SplitRcvMsgComponents(QByteArray)));
            connect(&NowSI2->mSerialPort, SIGNAL(readyRead()), this, SLOT(serialport24_read())); //emit receiveMsg signal
            //connect(uhv4_1, SIGNAL(SendMessage(QByteArray)), NowSI1, SLOT(simpleWriteNow(QByteArray)));
            //connect(thread_1, SIGNAL(started()), uhv4_1

            thread_2->start();
        }
    }
    else if (ui->checkBox2->isChecked() == false)
    {
       thread_2->terminate();
    }
}

void MainWindow::on_checkBox3_stateChanged(int state)
{
    QThread *thread_3;
    thread_3 = new QThread;

    if (ui->checkBox3->isChecked())
    {
        if (ui->radioButton31->isChecked())
        {
            qDebug() << "radio button checked" << endl;
            UHV2 *uhv2_3;
            uhv2_3 = new UHV2(UHV2::CountUHV2()+1);
            UHV2Tmp_3 = uhv2_3;
            qDebug() << "address la: " << QString::number(uhv2_3->GetUHV2Address(), 16) << endl;

            uhv2_3->moveToThread(thread_3);
            qDebug() << "Serial Interface: " << NowSI3->GetPortName() << endl;

            connect(this, SIGNAL(setSerialInterface(QString)), UHV2Tmp_3, SLOT(SetSerialInterface(QString)));
            emit setSerialInterface(NowSI3->mPortName); //set the serial interface for the object
            disconnect(this, SIGNAL(setSerialInterface(QString)), 0, 0);
            connect(this, SIGNAL(receiveMsg3(QByteArray)), UHV2Tmp_3, SLOT(SplitRcvMsgComponents(QByteArray)));
            connect(&NowSI3->mSerialPort, SIGNAL(readyRead()), this, SLOT(serialport32_read())); //emit receiveMsg signal
            //connect(uhv4_1, SIGNAL(SendMessage(QByteArray)), NowSI1, SLOT(simpleWriteNow(QByteArray)));
            //connect(thread_1, SIGNAL(started()), uhv4_1

            thread_3->start();
        }
        else if (ui->radioButton32->isChecked())
        {
            qDebug() << "radio button checked" << endl;
            UHV4 *uhv4_3;
            uhv4_3 = new UHV4(UHV4::CountUHV4()+1);
            UHV4Tmp_3 = uhv4_3;
            qDebug() << "address la: " << QString::number(uhv4_3->GetUHV4Address(), 16) << endl;

            uhv4_3->moveToThread(thread_3);
            qDebug() << "Serial Interface: " << NowSI3->GetPortName() << endl;

            connect(this, SIGNAL(setSerialInterface(QString)), UHV4Tmp_3, SLOT(SetSerialInterface(QString)));
            emit setSerialInterface(NowSI3->mPortName); //set the serial interface for the object
            disconnect(this, SIGNAL(setSerialInterface(QString)), 0, 0);
            connect(this, SIGNAL(receiveMsg3(QByteArray)), UHV4Tmp_3, SLOT(SplitRcvMsgComponents(QByteArray)));
            connect(&NowSI3->mSerialPort, SIGNAL(readyRead()), this, SLOT(serialport34_read())); //emit receiveMsg signal
            //connect(uhv4_1, SIGNAL(SendMessage(QByteArray)), NowSI1, SLOT(simpleWriteNow(QByteArray)));
            //connect(thread_1, SIGNAL(started()), uhv4_1

            thread_3->start();
        }
    }
    else if (ui->checkBox3->isChecked() == false)
    {
       thread_3->terminate();
    }
}

void MainWindow::on_checkBox4_stateChanged(int state)
{
    QThread *thread_4;
    thread_4 = new QThread;

    if (ui->checkBox4->isChecked())
    {
        if (ui->radioButton41->isChecked())
        {
            qDebug() << "radio button checked" << endl;
            UHV2 *uhv2_4;
            uhv2_4 = new UHV2(UHV2::CountUHV2()+1);
            UHV2Tmp_4 = uhv2_4;
            qDebug() << "address la: " << QString::number(uhv2_4->GetUHV2Address(), 16) << endl;

            uhv2_4->moveToThread(thread_4);
            qDebug() << "Serial Interface: " << NowSI4->GetPortName() << endl;

            connect(this, SIGNAL(setSerialInterface(QString)), UHV2Tmp_4, SLOT(SetSerialInterface(QString)));
            emit setSerialInterface(NowSI4->mPortName); //set the serial interface for the object
            disconnect(this, SIGNAL(setSerialInterface(QString)), 0, 0);
            connect(this, SIGNAL(receiveMsg4(QByteArray)), UHV2Tmp_4, SLOT(SplitRcvMsgComponents(QByteArray)));
            connect(&NowSI4->mSerialPort, SIGNAL(readyRead()), this, SLOT(serialport42_read())); //emit receiveMsg signal
            //connect(uhv4_1, SIGNAL(SendMessage(QByteArray)), NowSI1, SLOT(simpleWriteNow(QByteArray)));
            //connect(thread_1, SIGNAL(started()), uhv4_1

            thread_4->start();
        }
        else if (ui->radioButton42->isChecked())
        {
            qDebug() << "radio button checked" << endl;
            UHV4 *uhv4_4;
            uhv4_4 = new UHV4(UHV4::CountUHV4()+1);
            UHV4Tmp_4 = uhv4_4;
            qDebug() << "address la: " << QString::number(uhv4_4->GetUHV4Address(), 16) << endl;

            uhv4_4->moveToThread(thread_4);
            qDebug() << "Serial Interface: " << NowSI1->GetPortName() << endl;

            connect(this, SIGNAL(setSerialInterface(QString)), UHV4Tmp_4, SLOT(SetSerialInterface(QString)));
            emit setSerialInterface(NowSI4->mPortName); //set the serial interface for the object
            disconnect(this, SIGNAL(setSerialInterface(QString)), 0, 0);
            connect(this, SIGNAL(receiveMsg4(QByteArray)), UHV4Tmp_4, SLOT(SplitRcvMsgComponents(QByteArray)));
            connect(&NowSI4->mSerialPort, SIGNAL(readyRead()), this, SLOT(serialport44_read())); //emit receiveMsg signal
            //connect(uhv4_1, SIGNAL(SendMessage(QByteArray)), NowSI1, SLOT(simpleWriteNow(QByteArray)));
            //connect(thread_1, SIGNAL(started()), uhv4_1

            thread_4->start();
        }
    }
    else if (ui->checkBox4->isChecked() == false)
    {
       thread_4->terminate();
    }
}


void MainWindow::on_pushButton1_clicked()
{
    if (ui->radioButton11->isChecked())
    {
        emit setV2(ui->MaxV1->text().toInt(), ui->MinV1->text().toInt());
        emit setI2(ui->MaxI1->text().toDouble(), ui->MinI1->text().toDouble());
    }
    else if (ui->radioButton12->isChecked())
    {
        connect(this, SIGNAL(setV4(int, int)), UHV4Tmp_1,SLOT(SetRangeV(int,int)));
        connect(this, SIGNAL(setI4(double, double)), UHV4Tmp_1,SLOT(SetRangeI(double,double)));
        emit setV4(ui->MaxV1->text().toInt(), ui->MinV1->text().toInt());
        emit setI4(ui->MaxI1->text().toDouble(), ui->MinI1->text().toDouble());
    }

    disconnect(this, SIGNAL(setV2(int,int)), 0, 0);
    disconnect(this, SIGNAL(setV4(int,int)), 0, 0);
    disconnect(this, SIGNAL(setI2(double,double)), 0, 0);
    disconnect(this, SIGNAL(setI4(double,double)), 0, 0);
}

void MainWindow::on_pushButton2_clicked()
{
    if (ui->radioButton21->isChecked())
    {
        //connect
        emit setV2(ui->MaxV2->text().toInt(), ui->MinV2->text().toInt());
        emit setI2(ui->MaxI2->text().toDouble(), ui->MinI2->text().toDouble());
    }
    else if (ui->radioButton22->isChecked())
    {
        connect(this, SIGNAL(setV4(int, int)), UHV4Tmp_2,SLOT(SetRangeV(int,int)));
        connect(this, SIGNAL(setI4(double, double)), UHV4Tmp_2,SLOT(SetRangeI(double,double)));
        emit setV4(ui->MaxV2->text().toInt(), ui->MinV2->text().toInt());
        emit setI4(ui->MaxI2->text().toDouble(), ui->MinI2->text().toDouble());
    }

    disconnect(this, SIGNAL(setV2(int,int)), 0, 0);
    disconnect(this, SIGNAL(setV4(int,int)), 0, 0);
    disconnect(this, SIGNAL(setI2(double,double)), 0, 0);
    disconnect(this, SIGNAL(setI4(double,double)), 0, 0);
}

void MainWindow::on_pushButton3_clicked()
{
    if (ui->radioButton31->isChecked())
    {
        emit setV2(ui->MaxV3->text().toInt(), ui->MinV3->text().toInt());
        emit setI2(ui->MaxI3->text().toDouble(), ui->MinI3->text().toDouble());
    }
    else if (ui->radioButton32->isChecked())
    {
        connect(this, SIGNAL(setV4(int, int)), UHV4Tmp_3,SLOT(SetRangeV(int,int)));
        connect(this, SIGNAL(setI4(double, double)), UHV4Tmp_3,SLOT(SetRangeI(double,double)));
        emit setV4(ui->MaxV3->text().toInt(), ui->MinV3->text().toInt());
        emit setI4(ui->MaxI3->text().toDouble(), ui->MinI3->text().toDouble());
    }

    disconnect(this, SIGNAL(setV2(int,int)), 0, 0);
    disconnect(this, SIGNAL(setV4(int,int)), 0, 0);
    disconnect(this, SIGNAL(setI2(double,double)), 0, 0);
    disconnect(this, SIGNAL(setI4(double,double)), 0, 0);
}

void MainWindow::on_pushButton4_clicked()
{

    if (ui->radioButton41->isChecked())
    {
        emit setV2(ui->MaxV4->text().toInt(), ui->MinV4->text().toInt());
        emit setI2(ui->MaxI4->text().toDouble(), ui->MinI4->text().toDouble());
    }
    else if (ui->radioButton42->isChecked())
    {
        connect(this, SIGNAL(setV4(int, int)), UHV4Tmp_4,SLOT(SetRangeV(int,int)));
        connect(this, SIGNAL(setI4(double, double)), UHV4Tmp_4,SLOT(SetRangeI(double,double)));
        emit setV4(ui->MaxV4->text().toInt(), ui->MinV4->text().toInt());
        emit setI4(ui->MaxI4->text().toDouble(), ui->MinI4->text().toDouble());
    }

    disconnect(this, SIGNAL(setV2(int,int)), 0, 0);
    disconnect(this, SIGNAL(setV4(int,int)), 0, 0);
    disconnect(this, SIGNAL(setI2(double,double)), 0, 0);
    disconnect(this, SIGNAL(setI4(double,double)), 0, 0);
}

void MainWindow::serialport14_read()
{
    /// structure of valid msg: 1STX + xDATA + 1ETX + 2CRC
    Msg1.append(NowSI1->mSerialPort.read(20));
    qDebug() << "chua vo: " << Msg1.toHex() << endl;
    QByteArray a;
    a.append(UHV4::STX);
    a.append(UHV4Tmp_1->GetUHV4Address() + UHV4::AddressBase);

    if (Msg1.contains(a)) //wait for STX and valid address
    {
        Msg1.append(NowSI1->mSerialPort.readAll());
        qDebug() << "vo roi: " << Msg1.toHex() << endl;

        if (Msg1.contains((char) UHV4::ETX) && (Msg1.size()) > (Msg1.indexOf((char) UHV4::ETX) + 2)) //wait for ETX and 2 bytes CRC
        {
            qDebug() << "truoc khi cat: " << Msg1.toHex() << endl;

            int stx = Msg1.indexOf((char) UHV4::STX);
            qDebug() << "stx " << stx << endl;
            qDebug() << QString::number((UHV4Tmp_1->GetUHV4Address()) + UHV4::AddressBase, 16) << endl;
            while (Msg1.at(stx + 1) != (char) (UHV4Tmp_1->GetUHV4Address() + UHV4::AddressBase))
            {
                stx = Msg1.indexOf((char) UHV4::STX, stx + 1);
                qDebug() << "stx " << stx << "that nam o " <<  Msg1.indexOf(UHV4Tmp_1->GetUHV4Address() + UHV4::AddressBase) << " + 1 = " << Msg1.at(stx + 1) << endl;
            }
            int etx = Msg1.indexOf((char) UHV4::ETX, stx);
            qDebug() << "etx + 2 -stx +1= " << etx + 2 - stx + 1 << endl;
            if (Msg1.lastIndexOf((char) UHV4::ETX) != (Msg1.size() - 1) )
            {
                Msg1 = Msg1.mid(stx, etx + 2 - stx + 1);
                qDebug() << "result: " << Msg1.toHex() << endl;

                emit receiveMsg1(Msg1);
                Msg1.clear();
            }
        }
    }
    else
    {
        if(Msg1.lastIndexOf((char) UHV4::STX) != (Msg1.size() - 1)) //case chi doc dc STX
        {
            qDebug() << "bi xoa: " << Msg1.toHex() << endl;
            Msg1.clear();
        }
    }
    a.clear();
}

void MainWindow::serialport24_read()
{
    /// structure of valid msg: 1STX + xDATA + 1ETX + 2CRC
    Msg2.append(NowSI2->mSerialPort.read(20));
    qDebug() << "chua vo: " << Msg2.toHex() << endl;
    QByteArray a;
    a.append(UHV4::STX);
    a.append(UHV4Tmp_2->GetUHV4Address() + UHV4::AddressBase);

    if (Msg2.contains(a)) //wait for STX and valid address
    {
        Msg2.append(NowSI2->mSerialPort.readAll());
        qDebug() << "vo roi: " << Msg2.toHex() << endl;

        if (Msg2.contains((char) UHV4::ETX) && (Msg2.size()) > (Msg2.indexOf((char) UHV4::ETX) + 2)) //wait for ETX and 2 bytes CRC
        {
            qDebug() << "truoc khi cat: " << Msg2.toHex() << endl;

            int stx = Msg2.indexOf((char) UHV4::STX);
            qDebug() << "stx " << stx << endl;
            qDebug() << QString::number((UHV4Tmp_2->GetUHV4Address()) + UHV4::AddressBase, 16) << endl;
            while (Msg2.at(stx + 1) != (char) (UHV4Tmp_2->GetUHV4Address() + UHV4::AddressBase))
            {
                stx = Msg2.indexOf((char) UHV4::STX, stx + 1);
                qDebug() << "stx " << stx << "that nam o " <<  Msg2.indexOf(UHV4Tmp_2->GetUHV4Address() + UHV4::AddressBase) << " + 1 = " << Msg2.at(stx + 1) << endl;
            }
            int etx = Msg2.indexOf((char) UHV4::ETX, stx);
            qDebug() << "etx + 2 -stx +1= " << etx + 2 - stx + 1 << endl;
            if (Msg2.lastIndexOf((char) UHV4::ETX) != (Msg2.size() - 1) )
            {
                Msg2 = Msg2.mid(stx, etx + 2 - stx + 1);
                qDebug() << "result: " << Msg2.toHex() << endl;

                emit receiveMsg2(Msg2);
                Msg2.clear();
            }
        }
    }
    else
    {
        if(Msg2.lastIndexOf((char) UHV4::STX) != (Msg2.size() - 1)) //case chi doc dc STX
        {
            qDebug() << "bi xoa: " << Msg2.toHex() << endl;
            Msg2.clear();
        }
    }
    a.clear();
}

void MainWindow::serialport34_read()
{
    /// structure of valid msg: 1STX + xDATA + 1ETX + 2CRC
    Msg3.append(NowSI3->mSerialPort.read(20));
    qDebug() << "chua vo: " << Msg3.toHex() << endl;
    QByteArray a;
    a.append(UHV4::STX);
    a.append(UHV4Tmp_3->GetUHV4Address() + UHV4::AddressBase);

    if (Msg3.contains(a)) //wait for STX and valid address
    {
        Msg3.append(NowSI3->mSerialPort.readAll());
        qDebug() << "vo roi: " << Msg3.toHex() << endl;

        if (Msg3.contains((char) UHV4::ETX) && (Msg3.size()) > (Msg3.indexOf((char) UHV4::ETX) + 2)) //wait for ETX and 2 bytes CRC
        {
            qDebug() << "truoc khi cat: " << Msg3.toHex() << endl;

            int stx = Msg3.indexOf((char) UHV4::STX);
            qDebug() << "stx " << stx << endl;
            qDebug() << QString::number((UHV4Tmp_3->GetUHV4Address()) + UHV4::AddressBase, 16) << endl;
            while (Msg3.at(stx + 1) != (char) (UHV4Tmp_3->GetUHV4Address() + UHV4::AddressBase))
            {
                stx = Msg3.indexOf((char) UHV4::STX, stx + 1);
                qDebug() << "stx " << stx << "that nam o " <<  Msg3.indexOf(UHV4Tmp_3->GetUHV4Address() + UHV4::AddressBase) << " + 1 = " << Msg3.at(stx + 1) << endl;
            }
            int etx = Msg3.indexOf((char) UHV4::ETX, stx);
            qDebug() << "etx + 2 -stx +1= " << etx + 2 - stx + 1 << endl;
            if (Msg3.lastIndexOf((char) UHV4::ETX) != (Msg3.size() - 1) )
            {
                Msg3 = Msg3.mid(stx, etx + 2 - stx + 1);
                qDebug() << "result: " << Msg3.toHex() << endl;

                emit receiveMsg3(Msg3);
                Msg3.clear();
            }
        }
    }
    else
    {
        if(Msg3.lastIndexOf((char) UHV4::STX) != (Msg3.size() - 1)) //case chi doc dc STX
        {
            qDebug() << "bi xoa: " << Msg3.toHex() << endl;
            Msg3.clear();
        }
    }
    a.clear();
}

void MainWindow::serialport44_read()
{
    /// structure of valid msg: 1STX + xDATA + 1ETX + 2CRC
    Msg4.append(NowSI4->mSerialPort.read(20));
    qDebug() << "chua vo: " << Msg4.toHex() << endl;
    QByteArray a;
    a.append(UHV4::STX);
    a.append(UHV4Tmp_4->GetUHV4Address() + UHV4::AddressBase);

    if (Msg4.contains(a)) //wait for STX and valid address
    {
        Msg4.append(NowSI4->mSerialPort.readAll());
        qDebug() << "vo roi: " << Msg4.toHex() << endl;

        if (Msg4.contains((char) UHV4::ETX) && (Msg4.size()) > (Msg4.indexOf((char) UHV4::ETX) + 2)) //wait for ETX and 2 bytes CRC
        {
            qDebug() << "truoc khi cat: " << Msg4.toHex() << endl;

            int stx = Msg4.indexOf((char) UHV4::STX);
            qDebug() << "stx " << stx << endl;
            qDebug() << QString::number((UHV4Tmp_4->GetUHV4Address()) + UHV4::AddressBase, 16) << endl;
            while (Msg4.at(stx + 1) != (char) (UHV4Tmp_4->GetUHV4Address() + UHV4::AddressBase))
            {
                stx = Msg4.indexOf((char) UHV4::STX, stx + 1);
                qDebug() << "stx " << stx << "that nam o " <<  Msg4.indexOf(UHV4Tmp_4->GetUHV4Address() + UHV4::AddressBase) << " + 1 = " << Msg4.at(stx + 1) << endl;
            }
            int etx = Msg4.indexOf((char) UHV4::ETX, stx);
            qDebug() << "etx + 2 -stx +1= " << etx + 2 - stx + 1 << endl;
            if (Msg4.lastIndexOf((char) UHV4::ETX) != (Msg4.size() - 1) )
            {
                Msg4 = Msg4.mid(stx, etx + 2 - stx + 1);
                qDebug() << "result: " << Msg4.toHex() << endl;

                emit receiveMsg4(Msg4);
                Msg4.clear();
            }
        }
    }
    else
    {
        if(Msg4.lastIndexOf((char) UHV4::STX) != (Msg4.size() - 1)) //case chi doc dc STX
        {
            qDebug() << "bi xoa: " << Msg4.toHex() << endl;
            Msg4.clear();
        }
    }
    a.clear();
}

void MainWindow::serialport12_read()
{
    /// structure of valid msg: 1Hdr + 2Ln + 2Cmd + 1Ch + xDt + 1Cksm
    Msg1.append(NowSI1->mSerialPort.readAll());
    QByteArray a;
    a.append(UHV2Tmp_1->GetUHV2Address() + UHV2::HdrCmdBase);

    if (Msg1.contains(a))
    {
        Msg1.append(NowSI1->mSerialPort.readAll());

        while (Msg1.size() < 3)
        {
            Msg1.append(NowSI1->mSerialPort.readAll());
        }

        QByteArray Ln = Msg1.mid(1,2);
        int length = static_cast<int>(Ln[0] - '0')*10 + static_cast<int>(Ln[1] -'0');

        while (Msg1.size() < (1 + 2 + length + 1))
        {
            Msg1.append(NowSI1->mSerialPort.readAll());
        }

        int first = Msg1.indexOf(a);
        Msg1 = Msg1.mid(first, first + 1 + 2 + length + 1);

        emit receiveMsg1(Msg1);
        Msg1.clear();
    }
    else
    {
        Msg1.clear();
    }

    a.clear();
}

void MainWindow::serialport22_read()
{
    /// structure of valid msg: 1Hdr + 2Ln + 2Cmd + 1Ch + xDt + 1Cksm
    Msg2.append(NowSI2->mSerialPort.readAll());
    QByteArray a;
    a.append(UHV2Tmp_2->GetUHV2Address() + UHV2::HdrCmdBase);

    if (Msg2.contains(a))
    {
        Msg2.append(NowSI2->mSerialPort.readAll());

        while (Msg2.size() < 3)
        {
            Msg2.append(NowSI2->mSerialPort.readAll());
        }

        QByteArray Ln = Msg2.mid(1,2);
        int length = static_cast<int>(Ln[0] - '0')*10 + static_cast<int>(Ln[1] -'0');

        while (Msg2.size() < (1 + 2 + length + 1))
        {
            Msg2.append(NowSI2->mSerialPort.readAll());
        }

        int first = Msg2.indexOf(a);
        Msg2 = Msg2.mid(first, first + 1 + 2 + length + 1);

        emit receiveMsg2(Msg2);
        Msg2.clear();
    }
    else
    {
        Msg2.clear();
    }

    a.clear();
}

void MainWindow::serialport32_read()
{
    /// structure of valid msg: 1Hdr + 2Ln + 2Cmd + 1Ch + xDt + 1Cksm
    Msg3.append(NowSI3->mSerialPort.readAll());
    QByteArray a;
    a.append(UHV2Tmp_3->GetUHV2Address() + UHV2::HdrCmdBase);

    if (Msg3.contains(a))
    {
        Msg3.append(NowSI3->mSerialPort.readAll());

        while (Msg3.size() < 3)
        {
            Msg3.append(NowSI3->mSerialPort.readAll());
        }

        QByteArray Ln = Msg3.mid(1,2);
        int length = static_cast<int>(Ln[0] - '0')*10 + static_cast<int>(Ln[1] -'0');

        while (Msg3.size() < (1 + 2 + length + 1))
        {
            Msg3.append(NowSI3->mSerialPort.readAll());
        }

        int first = Msg3.indexOf(a);
        Msg3 = Msg3.mid(first, first + 1 + 2 + length + 1);

        emit receiveMsg3(Msg3);
        Msg3.clear();
    }
    else
    {
        Msg3.clear();
    }

    a.clear();
}

void MainWindow::serialport42_read()
{
    /// structure of valid msg: 1Hdr + 2Ln + 2Cmd + 1Ch + xDt + 1Cksm
    Msg4.append(NowSI4->mSerialPort.readAll());
    QByteArray a;
    a.append(UHV2Tmp_4->GetUHV2Address() + UHV2::HdrCmdBase);

    if (Msg4.contains(a))
    {
        Msg4.append(NowSI4->mSerialPort.readAll());

        while (Msg4.size() < 3)
        {
            Msg4.append(NowSI4->mSerialPort.readAll());
        }

        QByteArray Ln = Msg4.mid(1,2);
        int length = static_cast<int>(Ln[0] - '0')*10 + static_cast<int>(Ln[1] -'0');

        while (Msg4.size() < (1 + 2 + length + 1))
        {
            Msg4.append(NowSI4->mSerialPort.readAll());
        }

        int first = Msg4.indexOf(a);
        Msg4 = Msg4.mid(first, first + 1 + 2 + length + 1);

        emit receiveMsg4(Msg4);
        Msg4.clear();
    }
    else
    {
        Msg4.clear();
    }

    a.clear();
}


