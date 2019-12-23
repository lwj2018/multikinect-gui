#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include<iostream>
#include<stdio.h>
#include<stdlib.h>

typedef unsigned char uchar;

class PointCloud
{
public:
    PointCloud()
    {};
    float x = 0;
    float y = 0;
    float z = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

#endif // POINTCLOUD_H
