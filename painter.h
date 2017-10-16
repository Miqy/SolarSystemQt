#ifndef PAINTER_H
#define PAINTER_H

#include <mesh.h>

#include <QImage>
#include <vector>
#include <cmath>
#include <QColor>



class Painter
{
private:
    int width;
    int height;
    Point3D lightSource;
    double zBufor[600][600];
public:
    Camera* camera;
    Painter();
    Painter(int w, int h, Camera* cam);
    void setPixel(QImage *img, int x, int y, short red, short green, short blue);
    void drawLine(int x0, int y0, int x1, int y1, QImage *img);

    //simple texture triangle
    void textureTriangle(std::vector<QPoint> points, std::vector<QPoint> texturePoints, QImage *img, QImage *img2, double light);
    //texture triangle with light
    void textureTriangle(Mesh mesh, int a, int b, int c, std::vector<QPoint> points, std::vector<QPoint> texturePoints, QImage *img, QImage *img2, double light);

    //Phong shading
    //Args Points ABC and their normal vectors, point x,y and cuts x1, x2  with triangle lines
    Point3D phongShading(QPoint A, QPoint B, QPoint C, Point3D Na, Point3D Nb, Point3D Nc, double x, double y, double x1, double x2);

    void drawMesh(Mesh &mesh, QImage *img);
    void drawMesh(Mesh &mesh, QImage *img, QImage *texture, Camera *camera);

    //fucntions which texture wall of cube
    void textureWall(QPoint p1, QPoint p2, QPoint p3, QPoint p4, QImage *MainImage, QImage *TextureImage, double light);


    double countLightIntense(Point3D N, Point3D point);
    Point3D* getLightSource();

    void clearBufor();


};

#endif // PAINTER_H
