#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kinectsamplethread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(KinectSampleThread & t1,
               KinectSampleThread & t2,
               QWidget *parent = nullptr
               );
    ~MainWindow();
    KinectSampleThread &mt1;
    KinectSampleThread &mt2;
    int deviceId = 0;

public slots:
    void readFrame();
    void readDepthFrame();
    void checkoutDevice();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
