#ifndef TOOLS
#define TOOLS

#include <QMatrix4x4>
#include <cmath>


struct Point3D{
    double x, y, z;
        Point3D() : x(0), y(0), z(0){}
        Point3D(double v) : x(v), y(v), z(v) {}
        Point3D(double ix, double iy, double iz) : x(ix), y(iy), z(iz) {}
        Point3D operator +(const Point3D &other) const { return Point3D(x + other.x, y + other.y, z + other.z); }
        Point3D operator -(const Point3D &other) const { return Point3D(x - other.x, y - other.y, z - other.z); }
        Point3D operator *(const Point3D &other) const { return Point3D(x * other.x, y * other.y, z * other.z); }
        Point3D operator *(const double &other) const { return Point3D(x * other, y * other, z * other); }
        Point3D operator *(const int &other) const { return Point3D(x * other, y * other, z * other); }

        double dot(const Point3D &other){   //iloczyn skalarny
            return x * other.x + y * other.y + z * other.z;
        }
        Point3D normal(){
            double l = sqrt(dot(Point3D(x,y,z)));
            if(l == 0){
                return Point3D(0,0,0);
            }
            return Point3D(x/l, y/l, z/l);
        }
        Point3D cross(const Point3D &p){    //iloczyn wektorowy
            return Point3D(y * p.z - z * p.y,z * p.x - x * p.z, x * p.y - y * p.x);
        }
        //return vector from 2 points
        Point3D vector2Pts(Point3D p1, Point3D p2){
            return Point3D(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
        }
};

double trianglePole(Point3D A, Point3D B, Point3D C);
//funkcja sprawdzajaca dla punktu P i trojkata o bokach A i B oraaz wektora normalnego N po ktorej stornie bocznej plaszcyzny punkt P sie znajduje
double stronaPlaszczyzny(Point3D &P, Point3D &A, Point3D &B, Point3D &N);
QMatrix4x4 generateRotMat(double alfaX, double alfaY, double alfaZ);

#endif // TOOLS

