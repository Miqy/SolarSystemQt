#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <mesh.h>
#include <sphere.h>
#include <painter.h>
#include <algorithm>



class Scene
{
private:
    Camera* camera;
    Painter* painter;
    QImage* MainImage;
public:
    std::vector<Mesh*> Objects;
    //planets
    Sphere *sun,*mercury,*venus,*earth,*mars,*jowisz,*saturn,*uran,*neptun;
    //start position of planets
    Point3D mercPos, venPos, earthPos, marsPos, jowPos, satPos, uraPos, nepPos;

public:
    Scene();
    Scene(Painter *p, QImage* img);
    void refresh();
    void generateWorld(Mesh &root, QImage *Tex);

    //funkcje wprowadzajace ruch w uklad sloneczny
    void planetRound(double aX, double aY, double aZ, Sphere *planet, Point3D &stPos);
    void earthRound();

    Camera *getCamera() const;

};

#endif // SCENE_H
