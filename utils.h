#ifndef UTILS_H
#define UTILS_H
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

QImage rgbFrame2QImage(libfreenect2::Frame& frame);
QImage irFrame2QImage(libfreenect2::Frame& frame);
QImage depthFrame2QImage(libfreenect2::Frame& frame);

#endif // UTILS_H
