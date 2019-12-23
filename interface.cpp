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
#include <QDebug>
#include "interface.h"
#include "mslider.h"

Interface::Interface(KinectSampleThread & t1,
                     KinectSampleThread & t2):
    mt1(t1),mt2(t2)
{

    bool fs = false;
    QImage rgbImage(width,height,QImage::Format_RGB888);
    QImage depthImage(width,height,QImage::Format_Grayscale8);

    rgb = new QLabel();
    depth = new QLabel();
    rgb->setPixmap(QPixmap::fromImage(rgbImage));
    depth->setPixmap(QPixmap::fromImage(depthImage));
    pcWidget = new PointCloudGLWidget(mt1,mt2,fs);

    //! [1]
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);

    QTabWidget *displayTab = new QTabWidget();
    QWidget *rgbdCard = new QWidget;
    displayTab->addTab(rgbdCard,"RGB and depth");
    QHBoxLayout *rgbdLayout = new QHBoxLayout(rgbdCard);
    rgbdLayout->addWidget(rgb, 0,Qt::AlignLeft);
    rgbdLayout->addWidget(depth,0,Qt::AlignLeft);
    displayTab->addTab(pcWidget,"Point Cloud");

    QTabWidget *panel = new QTabWidget();
    QWidget *settingCard = new QWidget;
    QVBoxLayout *settingLayout = new QVBoxLayout(settingCard);
    panel->addTab(settingCard,"Settings");

    hLayout->addWidget(displayTab,0,Qt::AlignLeft);
    hLayout->addWidget(panel,0,Qt::AlignLeft);
    //! [1]

    widget->setWindowTitle(QStringLiteral("Multi-Kinect data sampler"));

    checkDevButton = new QPushButton();
    checkDevButton->setText(QString("checkout active device"));
    mSlider *xRotateSlider = new mSlider("rotate x",0,100,this);
    mSlider *yRotateSlider = new mSlider("rotate y",0,100,this);
    mSlider *zRotateSlider = new mSlider("rotate z",0,100,this);
    mSlider *xTranslateSlider = new mSlider("translate x",0,100,this);
    

    //! [4]
    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));
    themeList->setCurrentIndex(6);

    QPushButton *labelButton = new QPushButton(widget);
    labelButton->setText(QStringLiteral("Change label style"));

    QCheckBox *smoothCheckBox = new QCheckBox(widget);
    smoothCheckBox->setText(QStringLiteral("Smooth dots"));
    smoothCheckBox->setChecked(true);


    QPushButton *cameraButton = new QPushButton(widget);
    cameraButton->setText(QStringLiteral("Change camera preset"));

    QPushButton *itemCountButton = new QPushButton(widget);
    itemCountButton->setText(QStringLiteral("Toggle item count"));

    QCheckBox *backgroundCheckBox = new QCheckBox(widget);
    backgroundCheckBox->setText(QStringLiteral("Show background"));
    backgroundCheckBox->setChecked(true);

    QCheckBox *gridCheckBox = new QCheckBox(widget);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);

    QComboBox *shadowQuality = new QComboBox(widget);
    shadowQuality->addItem(QStringLiteral("None"));
    shadowQuality->addItem(QStringLiteral("Low"));
    shadowQuality->addItem(QStringLiteral("Medium"));
    shadowQuality->addItem(QStringLiteral("High"));
    shadowQuality->addItem(QStringLiteral("Low Soft"));
    shadowQuality->addItem(QStringLiteral("Medium Soft"));
    shadowQuality->addItem(QStringLiteral("High Soft"));
    shadowQuality->setCurrentIndex(4);

    QFontComboBox *fontList = new QFontComboBox(widget);
    fontList->setCurrentFont(QFont("Arial"));
    //! [4]

    //! [5]
    settingLayout->addWidget(checkDevButton,0,Qt::AlignTop);
    settingLayout->addWidget(xRotateSlider->self,0,Qt::AlignTop);
    settingLayout->addWidget(yRotateSlider->self,0,Qt::AlignTop);
    settingLayout->addWidget(zRotateSlider->self,0,Qt::AlignTop);
    settingLayout->addWidget(xTranslateSlider->self,0,Qt::AlignTop);

    settingLayout->addWidget(labelButton, 0, Qt::AlignTop);
    settingLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    settingLayout->addWidget(itemCountButton, 0, Qt::AlignTop);
    settingLayout->addWidget(backgroundCheckBox);
    settingLayout->addWidget(gridCheckBox);
    settingLayout->addWidget(smoothCheckBox, 0, Qt::AlignTop);
    settingLayout->addWidget(new QLabel(QStringLiteral("Change theme")));
    settingLayout->addWidget(themeList);
    settingLayout->addWidget(new QLabel(QStringLiteral("Adjust shadow quality")));
    settingLayout->addWidget(shadowQuality);
    settingLayout->addWidget(new QLabel(QStringLiteral("Change font")));
    settingLayout->addWidget(fontList, 1, Qt::AlignTop);
    //! [5]

    //! [6]

    //! [3]

    QTimer *timer = new QTimer();
    timer->start(20);


    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readDepthFrame()));
    QObject::connect(checkDevButton,SIGNAL(clicked()),this,SLOT(checkoutDevice()));
    QObject::connect(timer,SIGNAL(timeout()),pcWidget,SLOT(update()));
    QObject::connect(xRotateSlider->slider,SIGNAL(valueChanged(int)),this,SLOT(setxRotateValue(int)));
    QObject::connect(yRotateSlider->slider,SIGNAL(valueChanged(int)),this,SLOT(setyRotateValue(int)));
    QObject::connect(zRotateSlider->slider,SIGNAL(valueChanged(int)),this,SLOT(setzRotateValue(int)));
    QObject::connect(xTranslateSlider->slider,SIGNAL(valueChanged(int)),this,SLOT(setxTranslateValue(int)));



    widget->show();

}

void Interface::readFrame()
{
    int a =  width;
    int b =  height;
    QImage img;
    if(deviceId==0)
    {
        img = mt1.rgbImage.scaled(a,b);
    }
    else if (deviceId==1)
    {
        img = mt2.rgbImage.scaled(a,b);
    }
     rgb->setPixmap(QPixmap::fromImage(img));
}

void Interface::readDepthFrame()
{
    int a =  width;
    int b =  height;
    QImage img;
    if(deviceId==0)
    {
        img = mt1.depthImage.scaled(a,b);
    }
    else if (deviceId==1)
    {
        img = mt2.depthImage.scaled(a,b);
    }
     depth->setPixmap(QPixmap::fromImage(img));
}

void Interface::checkoutDevice()
{
    if(deviceId==0)
    {
        deviceId=1;
    }
    else if (deviceId==1)
    {
        deviceId=0;
    }
}

void Interface::setxRotateValue(int value)
{
    pcWidget->setxRotate(value);
}

void Interface::setyRotateValue(int value)
{
    pcWidget->setyRotate(value);
}

void Interface::setzRotateValue(int value)
{
    pcWidget->setzRotate(value);
}

void Interface::setxTranslateValue(int value)
{
    pcWidget->setxTranslate(value);
}
