#include "utils.h"

QImage rgbFrame2QImage(libfreenect2::Frame& frame)
{
    QImage image(frame.width,frame.height,QImage::Format_RGB888);
    // Copy input frame
    uchar *pSrc = frame.data;
    for(int row = 0; row < (int)frame.height; row ++)
    {
        uchar *pDest = image.scanLine(row);
        for(int col = 0; col<(int)frame.width;col++)
        {
            memcpy(pDest+3*col,pSrc,3);
            pSrc+=4;
        }
    }
    return image.rgbSwapped();

}

QImage depthFrame2QImage(libfreenect2::Frame& frame)
{
    QImage image(frame.width,frame.height,QImage::Format_Grayscale8);
    // Copy input frame
    uchar *pSrc = frame.data;
    float max = -9999999;
    for(int row = 0; row < (int)frame.height; row ++)
    {
        uchar *pDest = image.scanLine(row);
        for(int col = 0; col<(int)frame.width;col++)
        {
            float *val = new float;
            memcpy(val,pSrc,4);
            if(*val > max) {max=*val;}
            uchar grayVal = (uchar)((*val)/3600.0*255);
            if (grayVal>255) grayVal=255;
            else if (grayVal<0) grayVal=0;
            memcpy(pDest+col,&grayVal,1);
            pSrc+=4;
        }
    }
//    qDebug("max value:%f",max);
    return image.rgbSwapped();

}

QImage irFrame2QImage(libfreenect2::Frame& frame)
{
    QImage image(frame.width,frame.height,QImage::Format_Grayscale8);
    // Copy input frame
    uchar *pSrc = frame.data;
    for(int row = 0; row < (int)frame.height; row ++)
    {
        uchar *pDest = image.scanLine(row);
        for(int col = 0; col<(int)frame.width;col++)
        {
            float *val = new float;
            memcpy(val,pSrc,4);
            uchar grayVal = (uchar)((*val)/65535.0*255);
            memcpy(pDest+col,&grayVal,1);
            pSrc+=4;
        }
    }
    return image.rgbSwapped();

}

