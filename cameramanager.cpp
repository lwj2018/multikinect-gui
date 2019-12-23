
#include "cameramanager.h"
#include <algorithm>

Camera* Camera::camera = nullptr;

Camera::Camera()
{
    UpdateViewMatrix();

    ViewDir = eyeLocation - lookAtLocation;
    DefaultViewDir  = ViewDir;
}

void Camera::SetRotateXY(float x, float y)
{
    rotateX = x;
    rotateY = y;

    QMatrix4x4 rotation;
    rotation.rotate(rotateX, QVector3D(1,0,0));
    rotation.rotate(rotateY, QVector3D(0,1,0));

    ViewDir = DefaultViewDir * rotation;
    lookAtLocation = eyeLocation - ViewDir;
}

void Camera::UpdateViewMatrix()
{
    QVector3D upDir(0, 1, 0);

    QVector3D N = eyeLocation - lookAtLocation;
    QVector3D U = QVector3D::crossProduct(upDir, N);
    QVector3D V = QVector3D::crossProduct(N, U);

    N.normalize();
    U.normalize();
    V.normalize();

    viewMatrix.setRow(0, {U.x(), U.y(), U.z(), -QVector3D::dotProduct(U, eyeLocation)}); // x
    viewMatrix.setRow(1, {V.x(), V.y(), V.z(), -QVector3D::dotProduct(V, eyeLocation)}); // y
    viewMatrix.setRow(2, {N.x(), N.y(), N.z(), -QVector3D::dotProduct(N, eyeLocation)}); // z
    viewMatrix.setRow(3, {0, 0, 0, 1});
}

void Camera::MoveStep(QVector3D& dir, QVector3D step)
{
    dir.setX(dir.x() + step.x());
    dir.setZ(dir.z() + step.z());
}

void Camera::MoveLeft(float step)
{
    QVector3D upDir(0, 1, 0);
    QVector3D U = QVector3D::crossProduct(upDir, ViewDir);
    MoveStep(lookAtLocation, -step * U);
    MoveStep(eyeLocation, -step * U);

    UpdateViewMatrix();
}

void Camera::MoveRight(float step)
{
    QVector3D upDir(0, 1, 0);
    QVector3D U = QVector3D::crossProduct(upDir, ViewDir);

    MoveStep(lookAtLocation, step * U);
    MoveStep(eyeLocation, step * U);

    UpdateViewMatrix();
}

void Camera::MoveFront(float step)
{
    MoveStep(lookAtLocation, -step * ViewDir);
    MoveStep(eyeLocation, -step * ViewDir);

    UpdateViewMatrix();
}

void Camera::MoveBack(float step)
{
    MoveStep(lookAtLocation, step * ViewDir);
    MoveStep(eyeLocation, step * ViewDir);

    UpdateViewMatrix();
}

void Camera::MoveUp(float step)
{
    eyeLocation.setY(eyeLocation.y() + step);
    lookAtLocation.setY(lookAtLocation.y() + step);
    UpdateViewMatrix();
}

void Camera::MoveDown(float step)
{
    eyeLocation.setY(eyeLocation.y() - step);
    lookAtLocation.setY(lookAtLocation.y() - step);
    UpdateViewMatrix();
}

void Camera::Zoom(float step)
{
    lookAtLocation -= step * ViewDir;
    eyeLocation -= step * ViewDir;

    UpdateViewMatrix();
}
