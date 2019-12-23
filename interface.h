#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QWidget>
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
#include <QMainWindow>


class Interface: public QMainWindow
{
    Q_OBJECT
public:
    Interface(KinectSampleThread & t1,
              KinectSampleThread & t2);
    KinectSampleThread &mt1;
    KinectSampleThread &mt2;
    int deviceId = 0;
protected slots:
    void readFrame();
    void readDepthFrame();
    void checkoutDevice();
    void setxRotateValue(int value);
    void setyRotateValue(int value);
    void setzRotateValue(int value);
    void setxTranslateValue(int value);


protected:
    QLabel *rgb = nullptr;
    QLabel *depth = nullptr;
    QPushButton *checkDevButton = nullptr;
    PointCloudGLWidget *pcWidget = nullptr;
    static const int width = 500;
    static const int height = 500;
};

#endif // INTERFACE_H
