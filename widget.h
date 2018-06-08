#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

//串口设置
#include "qcom/qextserialbase.h"
#include "qcom/win_qextserialport.h"
#define TIME_OUT 2000        //延时，TIME_OUT是串口读写的延时
#define TIMER_INTERVAL 500  //读取定时器计时间间隔，500ms
//曲线绘制
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include "qcustomplot/qcustomplot.h"
//云平台搭建
#include <QSettings>
#include "yeelink/yeelinkapi_v1.h"
//视频采集
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <QtNetwork/QTcpSocket>
#include<QtNetwork/QTcpServer>
#include <iostream>
//视频处理
#include "opencv/dealpicture.h"
#include "opencv/imagetomat.h"
#include "opencv/mattoimage.h"
#include "opencv/qpixmaptoqimage.h"
#include <QDataStream>
#include <QFile>
#include <QDesktopServices>
#include <qsound.h>
using namespace std;
using namespace cv;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void SetCom();   //设置串口属性
    //云平台 ID和Key
    QString appKey;
    QString deviceId;
    void loadConfig();//读取传感器
    //定义一些数据变量
    YeelinkValueTypeSensor *yeelinkValueSensor;//温度
    YeelinkValueTypeSensor *yeelinkhumidity;//湿度
    YeelinkValueTypeSensor *yeelinkyanwu;//烟雾
    YeelinkValueTypeSensor *yeelinkgaungqiang;//光强
    YeelinkPhotoTypeSensor *yeelinkPhoto;//图片
    YeelinkBoolTypeSensor  *yeelinkBoolSensor1;//开关1
    YeelinkBoolTypeSensor  *yeelinkBoolSensor2;//开关2
    YeelinkBoolTypeSensor  *yeelinkBoolSensor3;//开关3

    bool BooL = true;
private slots:

    void Deal_video2();

    void Deal_video1();

    void save_pic();

    void Deal_video();

    void sendrequest();

    void read_video();

    void getData();//下载数据

    void UpData();//更新传值

    void upLoad();//上传数据

    void realtimeDataSlot();//数据曲线绘制

    void readMyCom();//读取串口的数据

    void on_OpenChuankou_clicked();

    void on_CloseChuankou_clicked();

    void on_ConfigBtn_clicked();

    void on_MonBtn_clicked();

    void on_BooL_clicked();

    void on_BooL_2_clicked();

    void on_VideoBtn_clicked();

    void on_VideoBtn_2_clicked();

    void on_yeelinkBtn_clicked();

    void on_VideoBtn_3_clicked();

    void on_VideoBtn_4_clicked();

private:
    Ui::Widget *ui;
    Win_QextSerialPort *myCom; //定义串口对象
    //曲线绘制
    QTimer *readTimerA;  //定时器A
    QTimer *dataTimer;   //用于定时绘制曲线的定时器
    //云平台上传数据
    QTimer *timer;
    QTimer *timer2;
    int timerdly = 0;
    int tempdata = 0;         //温度数据
    int humdata = 0;          //湿度数据
    int yandata = 0;
    int guangdata = 0;
    //void startInit();
    void setupRealtimeDataDemo();
private:
    //视频接收显示
    bool Lstop;
    Mat image;
    QTimer *timer1;

    QTcpSocket *tcpSocket;
    QTcpSocket  *Client;
    char lastbyte;
    bool enRecv;
    QByteArray block1;
    QByteArray qba;
    QImage *img;
    QPixmap Map1;

    void Connect_Video();
    //视频处理
    QPixmapToQImage map1;
    ImageToMat toMat;
    mattoimage toImage;
    DealPicture dealPicture;
    bool inCloneImageData = true;
    bool VD = true;
    bool VD1 = true;
    bool VD2 = true;

    QPixmap pix;
    QTimer *timer3;

public:
    int con ;
    int takephoto = 0;
    const char *filename = ".//qt.jpg" ;
    void Player();
    void Player1();
    QSound *son;
    QSound *son1;
};

#endif // WIDGET_H
