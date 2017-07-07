#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serialinterface.h>
#include <uhv4.h>
#include <uhv2.h>
#include <QThread>
#include <QObject>
#include <QByteArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    void InitMainWindow();

    SerialInterface *NowSI1;
    SerialInterface *NowSI2;
    SerialInterface *NowSI3;
    SerialInterface *NowSI4;

    UHV4 *UHV4Tmp_1;
    UHV4 *UHV4Tmp_2;
    UHV4 *UHV4Tmp_3;
    UHV4 *UHV4Tmp_4;

    UHV2 *UHV2Tmp_1;
    UHV2 *UHV2Tmp_2;
    UHV2 *UHV2Tmp_3;
    UHV2 *UHV2Tmp_4;

    QByteArray Msg1;
    QByteArray Msg2;
    QByteArray Msg3;
    QByteArray Msg4;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void setSerialInterface(QString serial);

    void setV2(int max, int min);
    void setI2(double max, double min);
    void setV4(int max, int min);
    void setI4(double max, double min);

    void receiveMsg1(QByteArray msg);
    void receiveMsg2(QByteArray &msg);
    void receiveMsg3(QByteArray &msg);
    void receiveMsg4(QByteArray &msg);

private slots:
    void on_comboBox1_currentIndexChanged(int index);
    void on_comboBox2_currentIndexChanged(int index);
    void on_comboBox3_currentIndexChanged(int index);
    void on_comboBox4_currentIndexChanged(int index);

    void on_checkBox1_stateChanged(int state);
    void on_checkBox2_stateChanged(int state);
    void on_checkBox3_stateChanged(int state);
    void on_checkBox4_stateChanged(int state);

    void on_pushButton1_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();
    void on_pushButton4_clicked();

public slots:
    //for UHV4
    void serialport14_read();
    void serialport24_read();
    void serialport34_read();
    void serialport44_read();
    //for UHV2
    void serialport12_read();
    void serialport22_read();
    void serialport32_read();
    void serialport42_read();
};

#endif // MAINWINDOW_H
