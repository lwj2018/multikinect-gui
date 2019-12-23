#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <iostream>
#include <QLabel>
#include <QTimer>
#include "utils.h"
#include <QThread>
#include <QDebug>
#include <QOpenGLWidget>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>
#include "pointcloudglwidget.h"


//KinectSampleThread t;

MainWindow::MainWindow(KinectSampleThread & t1,
                       KinectSampleThread & t2,
                       QWidget *parent
                       )
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
     , mt1(t1)
     , mt2(t2)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer();
    timer->start(20);

    connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));
    connect(timer, SIGNAL(timeout()), this, SLOT(readDepthFrame()));
    connect(ui->checkDevButton,SIGNAL(clicked()),this,SLOT(checkoutDevice()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFrame()
{
    int a = ui->rgb->width();
    int b = ui->rgb->height();
    QImage img;
    if(deviceId==0)
    {
        img = mt1.rgbImage.scaled(a,b);
    }
    else if (deviceId==1)
    {
        img = mt2.rgbImage.scaled(a,b);
    }
    ui->rgb->setPixmap(QPixmap::fromImage(img));
    ui->rgb->show();
}

void MainWindow::readDepthFrame()
{
    int a = ui->depth->width();
    int b = ui->depth->height();
    QImage img;
    if(deviceId==0)
    {
        img = mt1.depthImage.scaled(a,b);
    }
    else if (deviceId==1)
    {
        img = mt2.depthImage.scaled(a,b);
    }
    ui->depth->setPixmap(QPixmap::fromImage(img));
    ui->depth->show();
}

void MainWindow::checkoutDevice()
{
    qDebug()<<"enter checkout function";
    if(deviceId==0)
    {
        deviceId=1;
    }
    else if (deviceId==1)
    {
        deviceId=0;
    }
}



