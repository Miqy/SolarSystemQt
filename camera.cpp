#include "camera.h"

Camera::Camera()
{
    cX = 0;
    cY = -1000;
    cZ = 2500;

    tX = 15;
    tY = 0;
    tZ = 0;
    clear();


}

void Camera::clear()
{

    eX = -300;
    eY = -300;
    eZ = -1500;

    nX = 0;
    nY = 0;
    nZ = -1;

    wallVec = Point3D(1,0,0);
    wallVecY = Point3D(0,1,0);
}

void Camera::refresh()
{
    clear();
    double rad = tX/360.0 * 2 * M_PI;
    rad = -rad;
    QMatrix4x4 roX = QMatrix4x4(1,0,0,0,0,cos(rad),sin(rad),0,0,-sin(rad),cos(rad),0,0,0,0,1);

    rad = tY/360.0 * 2 * M_PI;
    rad = -rad;
    QMatrix4x4 roY = QMatrix4x4(cos(rad),0,-sin(rad),0,0,1,0,0,sin(rad),0,cos(rad),0,0,0,0,1);

    rad = tZ/360.0 * 2 * M_PI;
    rad = -rad;
    QMatrix4x4 roZ = QMatrix4x4(cos(rad),sin(rad),0,0,-sin(rad),cos(rad),0,0,0,0,1,0,0,0,0,1);

    rot = roX * roY * roZ;
    eZ = -1500 * -nZ;


    realN = Point3D(nX * rot(0,0) + nY * rot(0,1) + nZ * rot(0,2), nX * rot(1,0) + nY * rot(1,1) + nZ * rot(1,2), nX * rot(2,0) + nY * rot(2,1) + nZ * rot(2,2));
    realX = Point3D(wallVec.x * rot(0,0) + wallVec.y * rot(0,1) + wallVec.z * rot(0,2), wallVec.x * rot(1,0) + wallVec.y * rot(1,1) + wallVec.z * rot(1,2), wallVec.x * rot(2,0) + wallVec.y * rot(2,1) + wallVec.z * rot(2,2));
    realY = Point3D(wallVecY.x * rot(0,0) + wallVecY.y * rot(0,1) + wallVecY.z * rot(0,2), wallVecY.x * rot(1,0) + wallVecY.y * rot(1,1) + wallVecY.z * rot(1,2), wallVecY.x * rot(2,0) + wallVecY.y * rot(2,1) + wallVecY.z * rot(2,2));

    realN = realN.normal();
    realX = realX.normal();
    realY = realY.normal();

    realX = Point3D(realX.x/60, realX.y/60, realX.z/60);
    realY = Point3D(realY.x/60, realY.y/60, realY.z/60);

    rad = tX/360.0 * 2 * M_PI;
    roX = QMatrix4x4(1,0,0,0,0,cos(rad),sin(rad),0,0,-sin(rad),cos(rad),0,0,0,0,1);

    rad = tY/360.0 * 2 * M_PI;
    roY = QMatrix4x4(cos(rad),0,-sin(rad),0,0,1,0,0,sin(rad),0,cos(rad),0,0,0,0,1);

    rad = tZ/360.0 * 2 * M_PI;
    roZ = QMatrix4x4(cos(rad),sin(rad),0,0,-sin(rad),cos(rad),0,0,0,0,1,0,0,0,0,1);

    rot = roX * roY * roZ;




}

void Camera::setCameraLight(Point3D *lightSource)
{
    lightSource->x = cX;
    lightSource->y = cY;
    lightSource->z = cZ;
}



