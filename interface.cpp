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
//    QWidget *container = QWidget::createWindowContainer(&pcWidget);

    //! [1]
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();

    QTabWidget *tWidget = new QTabWidget();
    hLayout->addWidget(tWidget,0,Qt::AlignLeft);
    QWidget *rgbdCard = new QWidget;
    tWidget->addTab(rgbdCard,"RGB and depth");
    QHBoxLayout *rgbdLayout = new QHBoxLayout(rgbdCard);
    rgbdLayout->addWidget(rgb, 0,Qt::AlignLeft);
    rgbdLayout->addWidget(depth,0,Qt::AlignLeft);
    tWidget->addTab(pcWidget,"Point Cloud");
    hLayout->addLayout(vLayout);
    //! [1]

    widget->setWindowTitle(QStringLiteral("Multi-Kinect data sampler"));

    checkDevButton = new QPushButton();
    checkDevButton->setText(QString("checkout active device"));


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
    vLayout->addWidget(checkDevButton,0,Qt::AlignTop);
    vLayout->addWidget(labelButton, 0, Qt::AlignTop);
    vLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    vLayout->addWidget(itemCountButton, 0, Qt::AlignTop);
    vLayout->addWidget(backgroundCheckBox);
    vLayout->addWidget(gridCheckBox);
    vLayout->addWidget(smoothCheckBox, 0, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Change theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(new QLabel(QStringLiteral("Adjust shadow quality")));
    vLayout->addWidget(shadowQuality);
    vLayout->addWidget(new QLabel(QStringLiteral("Change font")));
    vLayout->addWidget(fontList, 1, Qt::AlignTop);
    //! [5]

    //! [6]

    //! [3]

    QTimer *timer = new QTimer();
    timer->start(20);


    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readDepthFrame()));
    QObject::connect(checkDevButton,SIGNAL(clicked()),this,SLOT(checkoutDevice()));
    QObject::connect(timer,SIGNAL(timeout()),pcWidget,SLOT(update()));

//    pcWidget->show();

//    if(viz_rgbdepth==0)
//    {
//        rgb->hide();
//        depth->hide();
//        pcWidget->show();
//    }
//    else if(viz_rgbdepth==1)
//    {
//        rgb->show();
//        depth->show();
//        pcWidget->hide();
//    }
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

void Interface::checkoutVizModal()
{
    qDebug("enter checkout viz");
    if(viz_rgbdepth==1)
    {
        viz_rgbdepth=0;
        rgb->hide();
        depth->hide();
        pcWidget->show();
    }
    else if(viz_rgbdepth==0)
    {
        viz_rgbdepth=1;
        rgb->show();
        depth->show();
        pcWidget->hide();
    }
}
