#ifndef MESH_H
#define MESH_H

#include <camera.h>

#include <vector>
#include <cmath>
#include <QPoint>
#include <QMatrix4x4>
#include <iostream>
#include <QImage>



class Mesh
{
private:
    //Macierze przeksztalcen
    QMatrix4x4 matrix;

    QMatrix4x4 transX;
    QMatrix4x4 transY;
    QMatrix4x4 transZ;

    QMatrix4x4 scalX;
    QMatrix4x4 scalY;
    QMatrix4x4 scalZ;

    QMatrix4x4 rotX;
    QMatrix4x4 rotY;
    QMatrix4x4 rotZ;

    QMatrix4x4 shrX;
    QMatrix4x4 shrY;
    QMatrix4x4 shrZ;

    bool isRoot;
    Mesh* parent;
    std::vector<Mesh*> childs;

public:
    //texture
    QImage* texture;

    std::vector<QPoint> points;
    std::vector<Point3D> vertices;
    std::vector<Point3D> Tnodes;    //Nodes after recalculation with matrix
    std::vector<long> triangles;
    std::vector<Point3D> NormVectors;   //Normal vectors of all triangles

    //texture pts and its iterator
    std::vector<QPoint> txtPs;
    int txtI = 0;

    double distanceToCamera;
public:
    Mesh();
    Mesh(Mesh*parent, QImage* tex, double length, double startX, double startY, double depth);


    //function which adds child to mesh
    void addChild(Mesh* child);

    //main functions of mesh
    void addTriangle(long a, long b, long c);
    void addQuad(long a, long b, long c, long d);
    void mapTo2D();
    void mapTo2D(Camera *camera);
    void refresh();
    void clear();

    virtual void initializeTexturePoints();

    //maths calculations
    void calculateNormVectors();
    Point3D countNormalVector(int i, int i2, int i3);
    Point3D countNormalVector(Point3D p1, Point3D p2, Point3D p3);
    Point3D interpolNormalVectorVertice(int a);
    Point3D countCamera3DPoint(Point3D a, Camera* camera);  //count camera 3d point before mapping it to 2d plane

    //matrix operations
    //functions returns all mutrix multiplied
    QMatrix4x4 multiplyMatrixes();

    void translateX(int r);
    void translateY(int r);
    void translateZ(int r);

    void scaleX(double s);
    void scaleY(double s);
    void scaleZ(double s);

    void rotateX(int alfa);
    void rotateY(int alfa);
    void rotateZ(int alfa);

    void shearingX(double s);
    void shearingY(double s);
    void shearingZ(double s);
};

#endif // MESH_H
