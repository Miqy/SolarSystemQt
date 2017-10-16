#include "scene.h"

Camera *Scene::getCamera() const
{
    return camera;
}

Scene::Scene()
{
    camera = new Camera();
}

Scene::Scene(Painter *p, QImage *img)
{
    camera = new Camera();
    painter = p;
    MainImage = img;

}

bool cmp(Mesh* a, Mesh* b){
    return a->distanceToCamera <  b->distanceToCamera;
}

void Scene::refresh()
{
    camera->refresh();
    painter->clearBufor();
    for(int i= 0; i < Objects.size(); i++){
        Objects[i]->refresh();
        Objects[i]->distanceToCamera = Objects[i]->countCamera3DPoint(Objects[i]->Tnodes[0],camera).z;
    }

    std::sort(Objects.begin(), Objects.end(),cmp);
    MainImage->fill(QColor(0,0,0,255));
    MainImage->load(":/images/sky.jpg");
    *MainImage = MainImage->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    for(int i= 0; i < Objects.size(); i++){
        Objects[i]->refresh();
        Objects[i]->mapTo2D(camera);
        painter->drawMesh(*Objects[i], MainImage, Objects[i]->texture, camera);
        Objects[i]->txtI = 0;
    }
    camera->setCameraLight(painter->getLightSource());
    //std::cout << camera->realN.x << ", " << camera->realN.y << ", " << camera->realN.z << "\n";
}

void Scene::generateWorld(Mesh &root, QImage* Tex)
{

    //Tworzenie scian ktore tworza kwadrtat, pomieszczenie
    QImage* Tex2 = new QImage(600, 600, QImage::Format_ARGB32);
    Tex2->load(":/images/texture2.jpg");
    *Tex2 = Tex2->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);

    QImage* SunTex = new QImage(600, 600, QImage::Format_ARGB32);


    //Tex2->fill(QColor(129,120,125,255));

    //Tworzenie scian dookola w trakcie kwadratu
    /*
    for(int i = -5; i <= 5; i++){
        Mesh *wall = new Mesh(&root, Tex2, 10, i * 10, 1, -50);
        Objects.push_back(wall);
    }

    for(int i = 0; i <= 10; i++){
        Mesh *wall = new Mesh(&root, Tex2, 10, 45, 1, -50 + i * 10);
        Objects.push_back(wall);
    }
    for(int i = -5; i <= 5; i++){
        Mesh *wall = new Mesh(&root, Tex2, 10, i * 10, 1, 40);
        Objects.push_back(wall);
    }
    for(int i = 0; i <= 10; i++){
        Mesh *wall = new Mesh(&root, Tex2, 10, -45, 1, -50 + i * 10);
        Objects.push_back(wall);
    }
    */

    //Tworzenie podlogi
    /*
    for(int i = -5; i <= 5; i++){
        for(int j = -5; j <= 5; j++){
            Mesh *wall = new Mesh(&root, Tex, 10, j * 10, 12, i * 10);
            Objects.push_back(wall);
        }
    }
    */
    int rou = 13;
    //Tworzenie wszystkich planet ukladu slonecznego
    SunTex->load(":/images/sun.jpg");
    *SunTex = SunTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //SunTex->fill(QColor(255,255,0,255));
    sun = new Sphere(SunTex,11,11,0,0,0,100);
    Objects.push_back(sun);

    QImage* MercTex = new QImage(600, 600, QImage::Format_ARGB32);
    MercTex->load(":/images/mercury.jpg");
    *MercTex = MercTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //MercTex->fill(QColor(240,100,7,255));
    mercPos = Point3D(0,0,-100);
    mercury = new Sphere(MercTex,rou,rou,mercPos.x,mercPos.y,mercPos.z,5);
    Objects.push_back(mercury);

    QImage* VenTex = new QImage(600, 600, QImage::Format_ARGB32);
    VenTex->load(":/images/venus.jpg");
    *VenTex = VenTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //VenTex->fill(QColor(255,0,0,255));
    venPos = Point3D(20,0,-200);
    venus = new Sphere(VenTex,rou,rou,venPos.x,venPos.y,venPos.z,10);
    Objects.push_back(venus);

    QImage* EarTex = new QImage(600, 600, QImage::Format_ARGB32);
    EarTex->load(":/images/earth.jpg");
    *EarTex = EarTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //EarTex->fill(QColor(60,160,200,255));
    earthPos = Point3D(0,0,-300);
    earth = new Sphere(EarTex,rou,rou,earthPos.x,earthPos.y,earthPos.z,10);
    Objects.push_back(earth);

    QImage* MarTex = new QImage(600, 600, QImage::Format_ARGB32);
    MarTex->load(":/images/mars.jpg");
    *MarTex = MarTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //MarTex->fill(QColor(82,40,3,255));
    marsPos = Point3D(0,0,-450);
    mars = new Sphere(MarTex,rou,rou,marsPos.x,marsPos.y,marsPos.z,12);
    Objects.push_back(mars);

    QImage* JowTex = new QImage(600, 600, QImage::Format_ARGB32);
    JowTex->load(":/images/jupiter.jpg");
    *JowTex = JowTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //JowTex->fill(QColor(48,48,30,255));
    jowPos = Point3D(0,0,-1500);
    jowisz = new Sphere(JowTex,rou,rou,jowPos.x,jowPos.y,jowPos.z,35);
    Objects.push_back(jowisz);

    QImage* SatTex = new QImage(600, 600, QImage::Format_ARGB32);
    SatTex->load(":/images/saturn.jpg");
    *SatTex = SatTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //SatTex->fill(QColor(200,200,200,255));
    satPos = Point3D(0,0,-2000);
    saturn = new Sphere(SatTex,rou,rou,satPos.x,satPos.y,satPos.z,35);
    Objects.push_back(saturn);

    QImage* UraTex = new QImage(600, 600, QImage::Format_ARGB32);
    UraTex->load(":/images/uran.jpg");
    *UraTex = UraTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //UraTex->fill(QColor(100,100,255,255));
    uraPos = Point3D(0,0,-2500);
    uran = new Sphere(UraTex,rou,rou,uraPos.x,uraPos.y,uraPos.z,25);
    Objects.push_back(uran);

    QImage* NepTex = new QImage(600, 600, QImage::Format_ARGB32);
    NepTex->load(":/images/neptun.jpg");
    *NepTex = NepTex->scaled(QSize(600, 600), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    //NepTex->fill(QColor(0,0,255,255));
    nepPos = Point3D(0,0,-3000);
    neptun = new Sphere(NepTex,rou,rou,nepPos.x,nepPos.y,nepPos.z,25);
    Objects.push_back(neptun);

    //tutaj mozna dodac pas asteroid
    /*
    Sphere* aster = new Sphere(MercTex,3,3,0,0,-900,50);
    aster->shearingX(0.1);
    aster->shearingY(0.05);
    aster->shearingZ(0.09);
    Objects.push_back(aster);
    */

    refresh();
}

void Scene::planetRound(double aX, double aY, double aZ, Sphere *planet, Point3D &stPos)
{
    QMatrix4x4 rot = generateRotMat(aX,aY,aZ);
    stPos = Point3D(stPos.x * rot(0,0) +stPos.y * rot(0,1) +stPos.z * rot(0,2),stPos.x * rot(1,0) +stPos.y * rot(1,1) +stPos.z * rot(1,2), stPos.x * rot(2,0) +stPos.y * rot(2,1) +stPos.z * rot(2,2));

    planet->translateX(stPos.x);
    planet->translateY(stPos.y);
    planet->translateZ(stPos.z);
}



void Scene::earthRound()
{
    QMatrix4x4 rot = generateRotMat(0.2,10,0.5);
    earthPos = Point3D(earthPos.x * rot(0,0) +earthPos.y * rot(0,1) +earthPos.z * rot(0,2),earthPos.x * rot(1,0) +earthPos.y * rot(1,1) +earthPos.z * rot(1,2), earthPos.x * rot(2,0) +earthPos.y * rot(2,1) +earthPos.z * rot(2,2));

    earth->translateX(earthPos.x);
    earth->translateY(earthPos.y);
    earth->translateZ(earthPos.z);

}

