#ifndef KINECTSAMPLETHREAD_H
#define KINECTSAMPLETHREAD_H

#include <QThread>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <iostream>
#include <QLabel>
#include <QTimer>
#include "pointcloud.h"

class KinectSampleThread
{
public:
    KinectSampleThread(std::string &mSerial);
    ~KinectSampleThread();

    libfreenect2::Freenect2Device *dev = 0;
    bool enable_rgb=true;
    bool enable_depth=true;
    int types=0;
    QImage rgbImage;
    QImage depthImage;
    QImage irImage;
    libfreenect2::SyncMultiFrameListener *listener = nullptr;
    libfreenect2::FrameMap frames;
    void run();
    std::string serial;
    std::vector<PointCloud> kCloud;
protected:
    void getPointCloud(libfreenect2::Frame &undistorted,
                                          libfreenect2::Frame &registered,
                                          libfreenect2::Registration& registration);
};

#endif // KINECTSAMPLETHREAD_H
