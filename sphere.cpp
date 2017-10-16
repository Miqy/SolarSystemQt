#include "sphere.h"

Sphere::Sphere(QImage* tex, int meridians, int parallels, double X, double Y, double Z, double size)
{
    meridian = meridians;
    parallel = parallels;
    clear();
    texture = tex;
    this->vertices.push_back(Point3D(0.0, 1.0, 0.0));
    for (long j = 0; j < parallels - 1; ++j)
    {
        double const polar = M_PI * double(j+1) / double(parallels);
        double const sp = std::sin(polar);
        double const cp = std::cos(polar);
        for (long i = 0; i < meridians; ++i)
        {
            double const azimuth = 2.0 * M_PI * double(i) / double(meridians);
            double const sa = std::sin(azimuth);
            double const ca = std::cos(azimuth);
            double const x = sp * ca;
            double const y = cp;
            double const z = sp * sa;
            this->vertices.push_back(Point3D(x, y, z));
        }
    }
    this->vertices.push_back(Point3D(0.0, -1.0, 0.0));

    for (long i = 0; i < meridians; ++i)
    {
        long const a = i + 1;
        long const b = (i + 1) % meridians + 1;
        this->addTriangle(0, b, a);
    }

    for (long j = 0; j < parallels - 2; ++j)
    {
        long aStart = j * meridians + 1;
        long bStart = (j + 1) * meridians + 1;
        for (long i = 0; i < meridians; ++i)
        {
            const long a = aStart + i;
            const long a1 = aStart + (i + 1) % meridians;
            const long b = bStart + i;
            const long b1 = bStart + (i + 1) % meridians;
            this->addQuad(a, a1, b1, b);
        }
    }

    for (long i = 0; i < meridians; ++i)
    {
        long const a = i + meridians * (parallels - 2) + 1;
        long const b = (i + 1) % meridians + meridians * (parallels - 2) + 1;
        this->addTriangle(this->vertices.size() - 1, a, b);
    }
    this->translateX(X);
    this->translateY(Y);
    this->translateZ(Z);
    this->scaleX(size);
    this->scaleY(size);
    this->scaleZ(size);
    this->rotateX(180);
    this->refresh();
    for(int i = 0;i < vertices.size(); i++){
        //std::cout << vertices[i].x << ", "<< vertices[i].y << ", " <<  vertices[i].z << "\n";
    }
    initializeTexturePoints();
}

void Sphere::initializeTexturePoints()
{
    txtPs.clear();
    txtI = 0;

    int w = texture->width()/meridian;
    int h = texture->height()/parallel;

    std::cout << w << "\n";

    for(int i = 0; i < meridian; i++){
        txtPs.push_back(QPoint(texture->width()/2,0));
        txtPs.push_back(QPoint((i+1)*w,h));
        txtPs.push_back(QPoint(i*w,h));
    }

    for(int i = 1; i < parallel-1; i++){
        for(int j = 0; j < meridian; j++){
            txtPs.push_back(QPoint(j * w,i *h));
            txtPs.push_back(QPoint((j+1) * w,i * h));
            txtPs.push_back(QPoint(j* w,(i + 1) *h));

            txtPs.push_back(QPoint((j + 1) * w,(i + 1) *h));
            txtPs.push_back(QPoint(j * w, (i+1)*h));
            txtPs.push_back(QPoint(j * w,i *h));
        }
    }
    for(int i = 0; i < meridian; i++){
        txtPs.push_back(QPoint(texture->width()/2,texture->height()));
        txtPs.push_back(QPoint(i*w,texture->height()-h));
        txtPs.push_back(QPoint((i+1)*w,texture->height()-h));
    }

}

