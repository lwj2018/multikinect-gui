#include "kinectsamplethread.h"
#include "utils.h"
#include <thread>
#include <QDebug>
namespace  {
    std::thread *_thread = nullptr;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
}
KinectSampleThread::KinectSampleThread ( std::string &mSerial)
{

    serial = mSerial;
    _thread = new std::thread([ = ] {run();});
    _thread->detach();

}

void KinectSampleThread::run()
{
    libfreenect2::Freenect2 freenect2;
    pthread_mutex_lock(&mutex);
    libfreenect2::PacketPipeline *pipeline = new libfreenect2::OpenGLPacketPipeline();
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex1);
    dev = freenect2.openDevice(serial,pipeline);
    pthread_mutex_unlock(&mutex1);
    if (dev == 0)
    {
        std::cout<< "failure opening device: " << serial << std::endl;
        return;
    }
    else
    {
        std::cout<< "success opening device: " << serial << std::endl;
    }

    if (enable_rgb)
      types |= libfreenect2::Frame::Color;
    if (enable_depth)
      types |= libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
    listener = new libfreenect2::SyncMultiFrameListener(types);



    dev->setColorFrameListener(listener);
    dev->setIrAndDepthFrameListener(listener);

    if (enable_rgb && enable_depth)
    {
      pthread_mutex_lock(&mutex2);
        if (!dev->start())
        return ;
      pthread_mutex_unlock(&mutex2);
    }
    else
    {
      if (!dev->startStreams(enable_rgb, enable_depth))
        return ;
    }
    std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
    
    libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4),depth2rgb(1920, 1080 + 2, 4);
    size_t framecount = 0;
    int protonect_shutdown = 0;
//    while(!protonect_shutdown && (framemax == (size_t)-1 || framecount < framemax))
    while(true)
        {

                    if (!listener->waitForNewFrame(frames, 10*1000)) // 10 sconds
                    {
                      std::cout << "timeout!" << std::endl;
                      return ;
                    }
          libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
          libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
          libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
          rgbImage = rgbFrame2QImage(*rgb);
//          qDebug("rgb: %s %d",serial.c_str(),rgb->timestamp);
          irImage = irFrame2QImage(*ir);
          depthImage = depthFrame2QImage(*depth);
//          qDebug("depth: %s %d",serial.c_str(),depth->timestamp);
          registration->apply(rgb, depth, &undistorted, &registered,false,&depth2rgb);
          getPointCloud(undistorted,registered,*registration);

          listener->release(frames);
          framecount += 1;
        }
//exec();

}

KinectSampleThread::~KinectSampleThread()
{
    dev->stop();
    dev->close();
    delete listener;
}

void KinectSampleThread::getPointCloud(libfreenect2::Frame &undistorted,
                                                          libfreenect2::Frame &registered,
                                                          libfreenect2::Registration& registration)
{
    std::vector<PointCloud> cloud;
    float x,y,z,color;
    
    for (int m = 0;  m < 512 ; m++)
    {
        for (int n = 0 ; n < 424 ; n++)
        {
            PointCloud* p = new PointCloud();
            registration.getPointXYZRGB(&undistorted, &registered, n, m, x, y, z, color);
            const uint8_t *c = reinterpret_cast<uint8_t*>(&color);
            uint8_t b = c[0];
            uint8_t g = c[1];
            uint8_t r = c[2];
//            if (z<1.2 && y<0.2)  //暂时先通过限定xyz来除去不需要的点，点云分割还在学习中。。。
//            {
                p->z = -z;
                p->x = x;
                p->y = -y;
                p->b = b;
                p->g = g;
                p->r = r;
//            }
            cloud.push_back(*p);
//            qDebug("%f,%f,%f,%f,%f,%f,%f",r,g,b,x,y,z);

        }
    }
    kCloud.assign(cloud.begin(), cloud.end());
}
