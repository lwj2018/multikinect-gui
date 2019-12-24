#include "mainwindow.h"

#include <QApplication>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <iostream>
#include <kinectsamplethread.h>
#include <QObject>
#include <QMessageBox>
#include "pointcloudglwidget.h"
#include <QTimer>
#include "interface.h"

int deviceId = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool fs = false;
//    switch( QMessageBox::information( 0,
//        "Start FullScreen?",
//        "Would You Like To Run In Fullscreen Mode?",
//        QMessageBox::Yes,
//        QMessageBox::No | QMessageBox::Default ) )
//    {
//    case QMessageBox::Yes:
//      fs = true;
//      break;
//    case QMessageBox::No:
//      fs = false;
//      break;
//    }

    std::string serial1("142057235047");
    std::string serial2("303029434147");
    std::string serial3("002138734747");


//    KinectSampleThread t1(serial1);
    KinectSampleThread t2(serial2);
    KinectSampleThread t3(serial3);


//    QTimer* timer = new QTimer();
//    timer->start(20);

//    PointCloudGLWidget pcWidget(t1,t2,fs);
//    QObject::connect(timer,SIGNAL(timeout()),&pcWidget,SLOT(update()));

//    pcWidget.show();

//    MainWindow w(t1,t2);
//    w.show();

    Interface gui(t2,t2,t3);

    return a.exec();
}


