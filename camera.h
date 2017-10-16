#ifndef CAMERA_H
#define CAMERA_H

#include <tools.h>


class Camera
{
public:
    double cX, cY, cZ;  //pozycja kamery
    double tX, tY, tZ;  //katy
    double eX, eY, eZ;  //pozycja obserwatora
    double nX, nY, nZ; //wektor normalny kamery
    Point3D wallVec;       //wektor rownolegly do plaszczyzny kamery
    Point3D wallVecY;      //wektor rownloegly do plaszczyzny kamery wzdluz osi Y

    Point3D realN;  //normalne wektory kamery po przemnozeniuu przez macierze
    Point3D realX;
    Point3D realY;

    QMatrix4x4 rot;


public:
    Camera();
    void clear();
    void refresh();
    void setCameraLight(Point3D *lightSource);  //metoda ustawiajaca zrodlo swiatla jak kamere

};

#endif // CAMERA_H
