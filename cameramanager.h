#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H
#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
private:
    QVector3D DefaultViewDir;
    QVector3D ViewDir = QVector3D(0, 0, 1);
    float rotateX = 0.0f;
    float rotateY = 0.0f;

    QVector3D eyeLocation = QVector3D(0, 1, 4);
    QVector3D lookAtLocation = QVector3D(0, 0, 0);
    QMatrix4x4 viewMatrix;

    void MoveStep(QVector3D& dir, QVector3D step);
    Camera();

public:
    static Camera* camera;
    static Camera* Inst() {
        if(!camera) {
            camera = new Camera();
        }
        return camera;
    }

    void SetRotateXY(float x, float y);
    float GetRotateX() { return rotateX; }
    float GetRotateY() { return rotateY; }

    void MoveLeft(float step);
    void MoveRight(float step);
    void MoveFront(float step);
    void MoveBack(float step);
    void MoveUp(float step);
    void MoveDown(float step);
    void Zoom(float step);

    void UpdateViewMatrix();
    const QMatrix4x4& GetViewMatrix() { return viewMatrix; }
    const QVector3D& GetCameraPos() { return eyeLocation; }
    const QVector3D& GetViewDir() { return ViewDir; }
};


#endif // CAMERA_H
