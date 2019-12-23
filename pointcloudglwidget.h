#ifndef POINTCLOUDGLWIDGET_H
#define POINTCLOUDGLWIDGET_H
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include "pointcloud.h"
#include "kinectsamplethread.h"



class PointCloudGLWidget: public  QOpenGLWidget
{
public:
    PointCloudGLWidget(KinectSampleThread & t1,
                       KinectSampleThread & t2,
                       bool fs);
    KinectSampleThread &mt1;
    KinectSampleThread &mt2;
    void setDeviceId(int id);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent( QKeyEvent *e ) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    bool fullscreen;
    std::vector<PointCloud> pCloud;
    int deviceId = 0;

private:
    float mx = 0,my = 0,ax = 0,ay = 0;
    bool isDown = false;

};

#endif // POINTCLOUDGLWIDGET_H
