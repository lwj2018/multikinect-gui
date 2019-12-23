#include "pointcloudglwidget.h"
#include<QDebug>
#include "cameramanager.h"
#include <algorithm>
#include <math.h>
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

PointCloudGLWidget::PointCloudGLWidget(KinectSampleThread & t1,
                                       KinectSampleThread & t2,
                                       bool fs):
    mt1(t1),mt2(t2)
{
      setGeometry( 0, 0, 640, 480 );
      fullscreen = fs;
      if ( fullscreen )
        showFullScreen();

}

void PointCloudGLWidget::initializeGL()
{
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void PointCloudGLWidget::paintGL()
{
    qDebug()<<"enter paintGL";

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
     glLoadIdentity();
     glTranslatef(0.0,0.0,xTranslate);
     glRotatef(xRotate,  1.0,  0.0,  0.0 );
     glRotatef(yRotate,  0.0,  1.0,  0.0 );
     glRotatef(zRotate,  0.0,  0.0,  1.0 );
     glBegin(GL_POINTS);
     float maxx,maxy,maxz;
     float minx,miny,minz;
     if(deviceId==0)
     {
         for (int i=0;i<mt1.kCloud.size();i++) {
             PointCloud p = mt1.kCloud[i];
//             qDebug("%f,%f,%f,%f,%f,%f,%f",p.r,p.g,p.b,p.x,p.y,p.z);
             glColor3f(p.r/255.0,p.g/255.0,p.b/255.0);
             glVertex3f(p.x,p.y,p.z);
             maxx = max(maxx,p.x);
             maxy = max(maxy,p.y);
             maxz = max(maxz,p.z);
             minx = min(minx,p.x);
             miny = min(miny,p.y);
             minz = min(minz,p.z);
         }
     }
     else if(deviceId==1)
     {
         for (int i=0;i<mt2.kCloud.size();i++) {
             PointCloud p = mt2.kCloud[i];
             glColor3f(p.r/255.0,p.g/255.0,p.b/255.0);
             glVertex3f(p.x,p.y,p.z);
         }
     }
//     qDebug("%f %f %f %f %f %f %f",maxx,maxy,maxz,minx,miny,minz);

     glEnd();


}

void PointCloudGLWidget::resizeGL(int w, int h)
{
    if ( h == 0 )
    {
      h = 1;
    }
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

}

void PointCloudGLWidget::keyPressEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {
  case Qt::Key_F2:
    fullscreen = !fullscreen;
    if ( fullscreen )
    {
      showFullScreen();
    }
    else
    {
      showNormal();
      setGeometry( 0, 0, 640, 480 );
    }
    break;
  case Qt::Key_Escape:
    close();
  }
}

void PointCloudGLWidget::setDeviceId(int id)
{
    deviceId = id;
}


void PointCloudGLWidget::mousePressEvent(QMouseEvent *e)
{
    isDown = true;
    float x = e->x();
    float y = e->y();
    mx = x;
    my = y;

    update();
}

void PointCloudGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(isDown)
    {
        float x = e->x();
        float y = e->y();

        ay += (x - mx) / 5.0f;
        mx = x;

        ax += (y - my) / 5.0f;
        my = y;

        // clamp(ax,-70,70)
        if(ax >= 70) ax = 70;
        else if(ax <= -70) ax = -70;

        Camera::Inst()->SetRotateXY(ax, ay);
        Camera::Inst()->UpdateViewMatrix();
        update();
    }
}

void PointCloudGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    isDown = false;
}

void PointCloudGLWidget::wheelEvent(QWheelEvent* event)
{
    float numDegrees = (float)event->delta() / 50;
    Camera::Inst()->Zoom(numDegrees);    update();
}

void PointCloudGLWidget::setxRotate(int rotate)
{
    zRotate = (float) rotate;
    paintGL();
}

void PointCloudGLWidget::setyRotate(int rotate)
{
    yRotate = (float) rotate;
    paintGL();
}

void PointCloudGLWidget::setzRotate(int rotate)
{
    zRotate = (float) rotate;
    paintGL();
}

void PointCloudGLWidget::setxTranslate(int value)
{
    xTranslate = (float) value/10.0;
    paintGL();
}



