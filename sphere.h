#ifndef SPHERE_H
#define SPHERE_H

#include <mesh.h>

class Sphere : public Mesh
{
private:
    int meridian,parallel;
public:
    Sphere(QImage *tex, int, int, double X, double Y, double Z, double size);
    virtual void initializeTexturePoints();
};

#endif // SPHERE_H
