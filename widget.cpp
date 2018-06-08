#include "widget.h"
#include "ui_widget.h"

QByteArray imageBuff;
QString yanwu = NULL;
QString guangqiang = NULL;
QString wendu = NULL ;
QString shidu = NULL;
int hongwai = 0;
QString w1 = NULL;
QString s1 = NULL;
QString g1 = NULL;
QString y2 = NULL;
int JD1 = 2; //继电器标识
int JD2 = 2;
int JD3 = 2;
int t1 = 0,t2 = 0;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("系统"));
    this->setWindowIcon(QIcon(":/image/2.jpg"));
    //设置界面背景
    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(QPixmap(":/image/4.jpg").scaled(this->size())));
    this->setPalette(palette);

    ui->stackedWidget->setCurrentIndex(0);
    //创建一个传感器对象
    yeelinkValueSensor = new YeelinkValueTypeSensor();
    yeelinkhumidity    = new YeelinkValueTypeSensor();
//    yeelinkyanwu       = new YeelinkValueTypeSensor();
    yeelinkgaungqiang  = new YeelinkValueTypeSensor();
//    yeelinkPhoto       = new YeelinkPhotoTypeSensor();
//    yeelinkBoolSensor1 = new YeelinkBoolTypeSensor();
//    yeelinkBoolSensor2 = new YeelinkBoolTypeSensor();
//    yeelinkBoolSensor3 = new YeelinkBoolTypeSensor();

   // timer2 = new QTimer(this);
    //connect(timer2,SIGNAL(timeout()),this,SLOT(UpData()));

     ui->VideoBtn->hide();
     ui->VideoBtn_2->hide();
     ui->VideoBtn_3->hide();
     ui->VideoBtn_4->hide();
     ui->yeelinkBtn->hide();
     ui->label_13->hide();
     //ui->label_14->hide();
     //ui->YlcdNumber->hide();


}

Widget::~Widget()
{
    delete ui;
//    delete yeelinkBoolSensor1;
//    delete yeelinkBoolSensor2;
//    delete yeelinkBoolSensor3;
//    delete yeelinkgaungqiang;
//    delete yeelinkhumidity;
//    delete yeelinkPhoto;
//    delete yeelinkValueSensor;
//    delete yeelinkyanwu;
//    delete timer2;
  //  delete timer;

}

void Widget::on_OpenChuankou_clicked() //打开串口
{
    QString str = ui->comboBox->currentText();
    myCom = new Win_QextSerialPort(str,QextSerialBase::EventDriven);
    myCom->open(QIODevice::ReadWrite);
    if(myCom->isOpen())
    {
        ui->Sta_label->setText(tr("串口打开成功"));
        SetCom();
        myCom->setTimeout(TIME_OUT);//设置延时
        ui->OpenChuankou->setEnabled(false);
        ui->CloseChuankou->setEnabled(true);
        setupRealtimeDataDemo();
        //timer->start(10000);
    }
    else{
        ui->Sta_label->setText(tr("串口打开失败"));
    }
}

void Widget::on_CloseChuankou_clicked() //关闭串口
{
    ui->Sta_label->setText(tr("串口关闭"));
    ui->OpenChuankou->setEnabled(true);
    ui->CloseChuankou->setEnabled(false);
    myCom->close();

//    dataTimer->stop();
//    if(timer->isActive())
//    {
//        timer->stop();
//    }

//    if(timer2->isActive())
//    {
//        timer2->stop();
//    }

//    Client->close();
}

void Widget::SetCom()//设置串口属性
{
    myCom->setBaudRate(BAUD115200);
    myCom->setDataBits(DATA_8);
    myCom->setParity(PAR_NONE);
    myCom->setStopBits(STOP_1);
    myCom->setFlowControl(FLOW_OFF);
    myCom->setTimeout(500); //延时500ms
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
}

void Widget::readMyCom() //读取串口数据
{
    bool ok; 
    myCom->setTimeout(500);
    if(myCom->bytesAvailable()>=18)
    {
        //读取串口缓冲区的所有数据给临时变量temp
        QByteArray temp = myCom->readAll();
        QString str1  = QString(temp);
        //str1.startsWith(y);
        qDebug()<<str1;
        wendu      = str1.mid(14,2);
        shidu      = str1.mid(17,2);
       // wendu      = "16";
        //shidu      = "68";
        yanwu      = str1.mid(1,3);
        guangqiang = str1.mid(6,2);

        tempdata = wendu.toInt(&ok);
        //tempdata = 16;
        humdata = shidu.toInt(&ok);
       // humdata = 68;
        yandata = yanwu.toInt(&ok);
       // guangdata = guangqiang.toInt(&ok);
        if(!str1.isEmpty())
        {

            t1 = str1.mid(4,1).toInt();
            if(t1 == 3 && yanwu.toInt()< 250)
            {
                ui->YlcdNumber->display(yanwu);
                yandata = yanwu.toInt(&ok);
                JD3     = str1.mid(11,1).toInt();
                hongwai   = str1.mid(10,1).toInt();
                yanwu = "0";
                y2 = yanwu;
                QString strY = "4";
                QByteArray byteY;
                if(BooL)
                {
                    if(y2.toInt()>30)
                    {
                        if( 1 == JD3)
                        {
                            strY = "4";
                            byteY = strY.toLatin1();
                            myCom->write(byteY);
                            qDebug()<<byteY;
                        }
                    }
                    else{
                        if( 0 == JD3)
                        {
                            strY = "5";
                            byteY = strY.toLatin1();
                            myCom->write(byteY);
                            qDebug()<<byteY;
                        }
                    }
                }

            }
            t2 = str1.mid(8,1).toInt();
            if(t2 == 2)
            {
                ui->GlcdNumber->display(guangqiang);
                guangdata = guangqiang.toInt(&ok);
                JD2     = str1.mid(11,1).toInt();
                g1 = guangqiang;
                QString strG = "2";
                QByteArray byteG;
                if(BooL)
                {
                    if(g1.toInt()>50)
                    {
                        if( 1 == JD2)
                        {
                            strG = "2";
                            byteG = strG.toLatin1();
                            myCom->write(byteG);
                            qDebug()<<byteG;
                        }

                     }else{
                        if(0 == JD2)
                        {
                            strG = "3";
                            byteG = strG.toLatin1();
                            myCom->write(byteG);
                            qDebug()<<byteG;
                        }

                    }
                }

            }
            if(wendu!="00")
            {
                ui->lcdNumber1->display(wendu);
                ui->lcdNumber2->display(shidu);
                w1 = wendu;
                s1 = shidu;
                QString strS= "0";
                QByteArray byteS;
                JD1        = str1.mid(11,1).toInt();
                if(BooL)
                {
                    if(s1.toInt()>80)
                    {
                        if( 1 == JD1)
                        {
                             strS = "0";
                             byteS = strS.toLatin1();
                             myCom->write(byteS);
                             qDebug()<<byteS;
                        }
                    }
                    else{
                        if(0 == JD1)
                        {
                            strS = "1";
                            byteS = strS.toLatin1();
                            myCom->write(byteS);
                            qDebug()<<byteS;
                        }
                    }
                }

            }
            if(hongwai == 1)
            {
                 //ui->Hlabel->setText(tr("有人"));
            }
            else
            {
                 //ui->Hlabel->setText(tr("无人"));
            }
        }
    }
}

void Widget::on_ConfigBtn_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_MonBtn_clicked()//检测界面
{
    ui->stackedWidget->setCurrentIndex(1);
    //getData();
//   timer = new QTimer(this);
//   connect(timer,SIGNAL(timeout()),this,SLOT(upLoad()));
//   timer->start(10000);
}

void Widget::setupRealtimeDataDemo()
{
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setName("temp");
   // ui->customPlot->graph(0)->setBrush(QBrush(QColor(205,133,63)));
    ui->customPlot->graph(0)->setAntialiasedFill(false);
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(1));
    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setPen(QPen(Qt::black));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(2));
    ui->customPlot->addGraph();
    ui->customPlot->graph(3)->setPen(QPen(Qt::yellow));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(3));


    ui->customPlot->addGraph();
    ui->customPlot->graph(4)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(4)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(4)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph();
    ui->customPlot->graph(5)->setPen(QPen(Qt::red));
    ui->customPlot->graph(5)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph();
    ui->customPlot->graph(6)->setPen(QPen(Qt::black));
    ui->customPlot->graph(6)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(6)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph();
    ui->customPlot->graph(7)->setPen(QPen(Qt::black));
    ui->customPlot->graph(7)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(7)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(0,100);
    connect(ui->customPlot->xAxis,SIGNAL(rangeChanged(QCPRange)),
            ui->customPlot->xAxis2,SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis,SIGNAL(rangeChanged(QCPRange)),
            ui->customPlot->yAxis2,SLOT(setRange(QCPRange)));
    dataTimer = new QTimer(this);
    connect(dataTimer,SIGNAL(timeout()),this,SLOT(realtimeDataSlot()));
    dataTimer->start(10);
}

void Widget::realtimeDataSlot()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastPointKey = 0;
    if(key-lastPointKey>0.01)
    {
        if(tempdata>0&&humdata>0)
        {
            ui->customPlot->graph(0)->addData(key,tempdata);
            ui->customPlot->graph(1)->addData(key,humdata);
            ui->customPlot->graph(2)->addData(key,yandata);
            ui->customPlot->graph(3)->addData(key,guangdata);

            ui->customPlot->graph(4)->clearData();
            ui->customPlot->graph(4)->addData(key,tempdata);
            ui->customPlot->graph(5)->clearData();
            ui->customPlot->graph(5)->addData(key,humdata);
            ui->customPlot->graph(6)->clearData();
            ui->customPlot->graph(6)->addData(key,yandata);
            ui->customPlot->graph(7)->clearData();
            ui->customPlot->graph(7)->addData(key,guangdata);

            ui->customPlot->graph(0)->removeDataBefore(key-8);
            ui->customPlot->graph(1)->removeDataBefore(key-8);
            ui->customPlot->graph(2)->removeDataBefore(key-8);
            ui->customPlot->graph(3)->removeDataBefore(key-8);

            ui->customPlot->graph(0)->rescaleValueAxis();
            ui->customPlot->graph(1)->rescaleValueAxis(true);
            ui->customPlot->graph(2)->rescaleValueAxis(true);
            ui->customPlot->graph(3)->rescaleValueAxis(true);
            lastPointKey = key;
        }

    }
    ui->customPlot->xAxis->setRange(key+0.5,10,Qt::AlignRight);
    ui->customPlot->replot();
}

void Widget::loadConfig() //读取传感器
{
    QSettings *config = new QSettings("yeelink.ini",QSettings::IniFormat);

    appKey = "d4ce17eb2774f37445b6d318f45c994c";
    deviceId = "352376";
    QString valueSensorId = "400965";
    QString humidity = "400966";
    QString yanwu = "403344";
    QString guangqiang = "400967";
//    QString photo = "400969";
//    QString bool1 = "406327";
//    QString bool2 = "406328";
//    QString bool3 = "406329";

    yeelinkValueSensor->init(deviceId,valueSensorId,appKey);  //初始化设备地址、传感器地址
    yeelinkhumidity->init(deviceId,humidity,appKey);
    yeelinkyanwu->init(deviceId,yanwu,appKey);
    yeelinkgaungqiang->init(deviceId,guangqiang,appKey);
    //yeelinkPhoto->init(deviceId,photo,appKey);
    //yeelinkBoolSensor1->init(deviceId,bool1,appKey);
    //yeelinkBoolSensor2->init(deviceId,bool2,appKey);
    //yeelinkBoolSensor3->init(deviceId,bool3,appKey);

    config->setValue("/app/apikey",appKey);
    config->setValue("/app/deviceid",deviceId);
    config->setValue("/sensor/valuesensor",valueSensorId);
    config->setValue("/sensor/valuesensor",humidity);
    config->setValue("/sensor/valuesensor",yanwu);
    config->setValue("/sensor/valuesensor",guangqiang);
//    config->setValue("/sensor/picsensor",photo);
//    config->setValue("/sensor/boolsensor",bool1);
//    config->setValue("/sensor/boolsensor",bool2);
//    config->setValue("/sensor/boolsensor",bool3);

}

void Widget::upLoad() //上传数据
{
    loadConfig();
    yeelinkValueSensor->setValTypeData(w1.toDouble());
    yeelinkhumidity->setValTypeData(s1.toDouble());
    yeelinkyanwu->setValTypeData(y2.toDouble());
    yeelinkgaungqiang->setValTypeData(g1.toDouble());
}



void Widget::on_BooL_clicked() //手动控制
{
    //getData();
    BooL = false;
    timer2->start(500);
}

void Widget::on_BooL_2_clicked() //自动控制
{
    if(timer2->isActive())
    {
        timer2->stop();
    }
    BooL = true;
}

void Widget::UpData()
{
    loadConfig();
    yeelinkBoolSensor1->getBoolStatus();
    yeelinkBoolSensor2->getBoolStatus();
    yeelinkBoolSensor3->getBoolStatus();

}

void Widget::getData()
{
    connect(yeelinkBoolSensor1,&YeelinkBoolTypeSensor::sig_boolData,[=](bool open){
        ui->lineEdit->setText(QString("%1").arg(open?"on":"off"));
        if(!BooL)
        {
            QString strS1= "0";
            QByteArray byteS1;
            if(open)
            {

                if( JD1 == 1)
                {
                     strS1 = "0";
                     byteS1 = strS1.toLatin1();
                     myCom->write(byteS1);
                     qDebug()<<byteS1;
                }

            }else{
                if(JD1 == 0)
                {
                    strS1 = "1";
                    byteS1 = strS1.toLatin1();
                    myCom->write(byteS1);
                    qDebug()<<byteS1;
                }
            }

        }
         });
    connect(yeelinkBoolSensor2,&YeelinkBoolTypeSensor::sig_boolData,[=](bool open){
        ui->lineEdit_2->setText(QString("%1").arg(open?"on":"off"));
        if(!BooL)
        {
            QString strS1= "2";
            QByteArray byteS1;
            if(open)
            {
                if( JD2 == 1)
                {
                     strS1 = "2";
                     byteS1 = strS1.toLatin1();
                     myCom->write(byteS1);
                     qDebug()<<byteS1;
                }
            }else{
                if(JD2 == 0)
                {
                    strS1 = "3";
                    byteS1 = strS1.toLatin1();
                    myCom->write(byteS1);
                    qDebug()<<byteS1;
                }
            }
        }
        });
    connect(yeelinkBoolSensor3,&YeelinkBoolTypeSensor::sig_boolData,[=](bool open){
        ui->lineEdit_3->setText(QString("%1").arg(open?"on":"off"));
        if(!BooL)
        {
            QString strS1= "4";
            QByteArray byteS1;
            if(open)
            {

                if( 1 == JD3)
                {
                     strS1 = "4";
                     byteS1 = strS1.toLatin1();
                     myCom->write(byteS1);
                     qDebug()<<byteS1;
                }

            }else{
                if(0 == JD3)
                {
                    strS1 = "5";
                    byteS1 = strS1.toLatin1();
                    myCom->write(byteS1);
                    qDebug()<<byteS1;
                }
            }

        }
         });
}

void Widget::on_VideoBtn_clicked()
{
     ui->stackedWidget->setCurrentIndex(2);
     timer3 = new QTimer(this);
     connect(timer3,SIGNAL(timeout()),this,SLOT(save_pic()));
     timer3->start(15000);
     Connect_Video();
     //if(timer->isActive())
    // {
     //    timer->stop();
    // }
//     myCom->close();
    // timer->stop();
//     dataTimer->stop();
//     if(timer2->isActive())
//     {
//         timer2->stop();
//     }
}

void Widget::sendrequest()
{
    QString str("GET /?action=stream");
    QByteArray qta = str.toLatin1();
    qta +=0x0A;
    qta +=0x0A;
    Client->write(qta);
}

void Widget::read_video()
{
    qba= Client->readAll(); //读取
    for(int i = 0;i < qba.count();i++)
    {
        if(enRecv) imageBuff += qba[i]; //将数据储存在数组中
        if(lastbyte == (char)0xFF)  //初始化为0
        {
            if(qba[i] == (char)0xD8)    //代表数据还没有接受完
            {
                enRecv = true;
            }
            else if(qba[i] == (char)0xD9)   //代表数据已经接受完
            {          
                if(Map1.loadFromData(imageBuff,"JEPG"))  //接受的图片格式是JEPG
                {           
                    ui->video->setPixmap(Map1); //显示图片
                    if(takephoto = 1)
                    {
                        Map1.save(filename);
                    }

                    if(!VD)
                    {
                       Deal_video();
                    }
                    if(!VD1)
                    {
                        Deal_video1();
                    }
                    if(!VD2)
                    {
                        Deal_video2();
                    }
                }
                imageBuff.clear();  //接受下一帧数据，要清除buf
                imageBuff += 0xFF;
                imageBuff += 0xD8;  //标志位，方便下一次接受
                enRecv = false;
                takephoto = 0;
            }
        }
        lastbyte = qba[i];
    }
}

void Widget::Connect_Video() //连接视频库
{
    Client = new QTcpSocket(this);
    Client->abort();
    QString ip = "192.168.1.230";
    int port = 8080;
    Client->connectToHost(ip,port);
    connect(Client,SIGNAL(connected()),this,SLOT(sendrequest()));
    connect(Client,SIGNAL(readyRead()),this,SLOT(read_video()));
}
void Widget::Deal_video()
{
    QImage image1 = map1.qpixmaptoqImage(Map1);
    Mat matt1 = toMat.imageToMat(image1,inCloneImageData);
    Mat matt3;

    if(!matt1.empty())
    {

        matt3 = dealPicture.Detection(matt1,con);
        QImage image2 = toImage.cvMatTOImage(matt3);
        ui->Deal_video->setPixmap(QPixmap::fromImage(image2));
    }
    if(con == 1)
    {
        ui->War_Label->setText(tr("有人请注意"));
        //Player1();
    }else {
        ui->War_Label->setText(tr("无人"));
    }
    con = 0;
}

void Widget::on_VideoBtn_2_clicked()
{   

    VD2 = true;
    VD1 = true;
    if(timer3->isActive())
    {
        timer3->stop();
    }
    if(VD)
    {
        VD = false;
    }


}

void Widget::save_pic()
{
    takephoto = 1;
    loadConfig();
    char *buf;
    QFile file(filename);
    file.open(QIODevice::ReadWrite);
    qint64 file_len = file.size();
    QDataStream in(&file);
    buf = new char[file_len];
    in.readRawData(buf,file_len);
    file.close();
    QByteArray arr = QByteArray(buf,file_len);
    yeelinkPhoto->setPhotoTypeData(arr);
}

void Widget::on_yeelinkBtn_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.yeelink.net/devices/352376"));
}

void Widget::on_VideoBtn_3_clicked()
{
    VD = true;
    VD2 = true;
    VD1 = false;
}

void Widget::Deal_video1()
{
    QImage image1 = map1.qpixmaptoqImage(Map1);
    Mat matt1 = toMat.imageToMat(image1,inCloneImageData);
    Mat matt3;

    if(!matt1.empty())
    {

        matt3 = dealPicture.DealHand(matt1,con);
        QImage image2 = toImage.cvMatTOImage(matt3);
        ui->Deal_video->setPixmap(QPixmap::fromImage(image2));
    }
    QString strS1= "0";
    QByteArray byteS1;
    if(con == 1)
    {

        if( 1 == JD1)
        {
             strS1 = "0";
             byteS1 = strS1.toLatin1();
             myCom->write(byteS1);
             qDebug()<<byteS1;
        }
        ui->War_Label->setText(tr("开灯"));

    }else if(con == 0){
        if(0 == JD1)
        {
            strS1 = "1";
            byteS1 = strS1.toLatin1();
            myCom->write(byteS1);
            qDebug()<<byteS1;
        }
        ui->War_Label->setText(tr("关灯"));
    }
}

void Widget::on_VideoBtn_4_clicked()
{
         VD = true;
         VD1 = true;
         VD2 = false;
}

void Widget::Deal_video2()
{
    QImage image1 = map1.qpixmaptoqImage(Map1);
    Mat matt1 = toMat.imageToMat(image1,inCloneImageData);
    Mat matt3;
    Mat src;
    if(!matt1.empty())
    {
        src = dealPicture.DealFire(matt1);
        matt3 = dealPicture.DrawFire(matt1,src,con);
        QImage image2 = toImage.cvMatTOImage(matt3);
        ui->Deal_video->setPixmap(QPixmap::fromImage(image2));
    }
    QString strS1= "0";
    QByteArray byteS1;
    if(con == 1&&yandata>30)
    {

        if( 1 == JD3)
        {
             strS1 = "4";
             byteS1 = strS1.toLatin1();
             //myCom->write(byteS1);
             qDebug()<<byteS1;
        }
        ui->War_Label->setText(tr("火焰警报"));
        Player();
    }else if(con == 0){
        if(0 == JD3)
        {
            strS1 = "5";
            byteS1 = strS1.toLatin1();
            //myCom->write(byteS1);
            qDebug()<<byteS1;
        }
        ui->War_Label->setText(tr("安全"));
    }
}

void Widget::Player()
{
    son = new QSound("C:\\Users\\SteveJobws\\Desktop\\33.wav");
    son->play();
}

void Widget::Player1()
{
    son1 = new QSound("C:\\Users\\SteveJobws\\Desktop\\22.wav");
    son1->play();
}
