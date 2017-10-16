#include "painter.h"

Point3D *Painter::getLightSource()
{
    return &lightSource;
}

void Painter::clearBufor()
{
    for(int i = 0; i < 600; i++){
        for(int j = 0; j < 600; j++){
            zBufor[i][j] = 3200000;
        }
    }
}

Painter::Painter()
{

}

Painter::Painter(int w, int h,Camera *cam)
{
    width = w;
    height = h;
    lightSource = Point3D(0,0,0);
    camera = cam;
    clearBufor();
}


void Painter::drawMesh(Mesh &mesh, QImage *img, QImage *texture, Camera* camera)
{

    for(int i = 0; i < mesh.triangles.size(); i+=3){
        int idx0 = mesh.triangles[i];
        int idx1 = mesh.triangles[i+1];
        int idx2 = mesh.triangles[i+2];
        QPoint p1 = mesh.points[idx0];
        QPoint p2 = mesh.points[idx1];
        QPoint p3 = mesh.points[idx2];

        Point3D p3d_0 = mesh.countCamera3DPoint(mesh.Tnodes[idx0], camera);
        Point3D p3d_1 = mesh.countCamera3DPoint(mesh.Tnodes[idx1], camera);
        Point3D p3d_2 = mesh.countCamera3DPoint(mesh.Tnodes[idx2], camera);

        Point3D v1 = p3d_1 - p3d_0;
        Point3D v2 = p3d_2 - p3d_0;

        Point3D Na = v1.cross(v2);

        double scalarTriCam = (Na.x * p3d_0.x + Na.y * p3d_0.y + Na.z * p3d_0.z)/(sqrt(Na.x * Na.x + Na.y * Na.y + Na.z * Na.z) * sqrt(p3d_0.x * p3d_0.x + p3d_0.y * p3d_0.y + p3d_0.z * p3d_0.z));  //cosinus kata miedzy wektorem kamery i wektora normalnego trojkata

        //std::cout << camera->nX << "," << camera->nY << "," << camera->nZ << "\n";

        std::vector<QPoint> tex;
        tex.push_back(mesh.txtPs[mesh.txtI]);
        tex.push_back(mesh.txtPs[mesh.txtI + 1]);
        tex.push_back(mesh.txtPs[mesh.txtI + 2]);
        if(mesh.txtI + 3 >= mesh.txtPs.size()){
            mesh.txtI = 0;
        }else{
            mesh.txtI += 3;
        }

        if(scalarTriCam < 0 && (p3d_0.z <= 0 || p3d_1.z <= 0 || p3d_2.z <= 0)){
            //if(wn.z > 0){
            std::vector<QPoint> pts;
            pts.push_back(p1);
            pts.push_back(p2);
            pts.push_back(p3);



            double light = 0;
            //Cieniowanie plaskie, liczenie swiatla raz dla kazdego trojkata
            //std::cout << "Normal: (" << Na.x << ", " <<  Na.y << ", " << Na.z << ")\n";
            Point3D Na = mesh.countNormalVector(idx0, idx1, idx2);
            light = countLightIntense(Na,Point3D((mesh.Tnodes[idx0].x + mesh.Tnodes[idx1].x + mesh.Tnodes[idx2].x)/3, (mesh.Tnodes[idx0].y + mesh.Tnodes[idx1].y + mesh.Tnodes[idx2].y)/3,(mesh.Tnodes[idx0].z + mesh.Tnodes[idx1].z + mesh.Tnodes[idx2].z)/3));
            //light = 0;
            //texture mesh

            textureTriangle(mesh, idx0, idx1, idx2, pts,tex,texture, img,light);

            //std::cout << "Trojkat: " << idx0 << ", " << idx1 << ", " <<  idx2 << ";\n";
            //std::cout << "Swiatlo: " << light << "\n";

            //drawing triangles mesh
            //drawLine(p1.x(), p1.y(), p2.x(), p2.y(), img);
            //drawLine(p1.x(), p1.y(), p3.x(), p3.y(), img);
            //drawLine(p2.x(), p2.y(), p3.x(), p3.y(), img);
            pts.clear();
        }
    }
}



double Painter::countLightIntense(Point3D N, Point3D point)
{
    //counting light intensity
    double I;   //mainLight
    double Ia, Ip;
    double Ka, Ks, Kd;
    double Fatt;    //rozpraszanie swiatla
    int n = 50;          //wspolczynnik gladkosci
    Point3D R;  //vector for light reflect
    Point3D L;  //vector to light source
    Point3D V;  //vector to observer

    L = Point3D(lightSource.x - point.x,lightSource.y - point.y,lightSource.z/100 - point.z/100);
    V = Point3D(camera->cX - point.x, camera->cY - point.y, camera->cZ - point.z);
    double Nol = N.x * L.x + N.y * L.y + N.z * L.z; //scalar of N&L vectors
    R = Point3D(L.x - 2 * Nol, L.y - 2 * Nol , L.z - 2 * Nol);
    double cosRoV = (R.x * V.x + R.y * V.y + R.z * V.z)/(sqrt(R.x * R.x + R.y * R.y + R.z * R.z) * sqrt(V.x * V.x + V.y * V.y + V.z * V.z));   //cosinus angle between vectors R and V
    Ia = 200;
    Ka = 0.20;
    Fatt = 0.001;
    Ip = 100;
    Kd = 0.65;
    Ks = 0.15;

    I = Ia * Ka + Fatt * Ip * Kd * Nol + Fatt * Ip * Ks * pow(cosRoV,n);
    //std::cout << "Scalar: " << Nol << ", Swiatlo: " << I << "!\n";
    return -I;
}

double minim(double a, double b, double c){
    double min = a;
    if(b < min) min = b;
    if(c < min) min = c;
    return min;

}

double maxim(double a, double b, double c){
    double maxi  = a;
    if(b > maxi) maxi = b;
    if(c > maxi) maxi = c;
    return maxi;
}

//Texture triangle with light
void Painter::textureTriangle(Mesh mesh, int a, int b, int c, std::vector<QPoint> points, std::vector<QPoint> texturePoints, QImage *img, QImage *img2, double light)
{
    double u,v,w;
    QColor kolor;

    double Xa = points[0].x();
    double Xb = points[1].x();
    double Xc = points[2].x();

    double Ya = points[0].y();
    double Yb = points[1].y();
    double Yc = points[2].y();

    double div = (Xb - Xa)  * (Yc - Ya) - (Yb- Ya) * (Xc - Xa);
    if(div == 0.0){
        return;
    }

    //count Normal vector of vertices abc for phongshading
    /*
    Point3D Na = mesh.interpolNormalVectorVertice(a);
    Point3D Nb = mesh.interpolNormalVectorVertice(b);
    Point3D Nc = mesh.interpolNormalVectorVertice(c);
    */

    //std::cout << "(" << Na.x << ", " << Na.y << ", " << Na.z << ")\n";

    //wierzcholki trojkata w odniesieniu do kameryt
   // Point3D A = mesh.countCamera3DPoint(mesh.Tnodes[a],camera);
    //Point3D B = mesh.countCamera3DPoint(mesh.Tnodes[b],camera);
    //Point3D C = mesh.countCamera3DPoint(mesh.Tnodes[c],camera);

    //Wektor normalny sciany trojkata renderowanego
    Point3D wall = mesh.countNormalVector(mesh.Tnodes[a],mesh.Tnodes[b], mesh.Tnodes[c]);

    //Point3D wall = mesh.countNormalVector(mesh.Tnodes[a], mesh.Tnodes[b], mesh.Tnodes[c]);

    //obliczenia ddla wyznaczenia rownania plaszczyzny trojkata
    /*  teraz bezuyteczne
    double wyzX = mesh.Tnodes[a].y * mesh.Tnodes[b].z + mesh.Tnodes[b].y * mesh.Tnodes[c].z + mesh.Tnodes[c].y * mesh.Tnodes[a].z - mesh.Tnodes[b].z * mesh.Tnodes[c].y - mesh.Tnodes[c].z * mesh.Tnodes[a].y - mesh.Tnodes[a].z * mesh.Tnodes[b].y;
    double wyzY = mesh.Tnodes[a].x * mesh.Tnodes[b].z + mesh.Tnodes[b].x * mesh.Tnodes[c].z + mesh.Tnodes[c].x * mesh.Tnodes[a].z - mesh.Tnodes[b].z * mesh.Tnodes[c].x - mesh.Tnodes[c].z * mesh.Tnodes[a].x - mesh.Tnodes[a].z * mesh.Tnodes[b].x;
    double wyzZ = mesh.Tnodes[a].x * mesh.Tnodes[b].y + mesh.Tnodes[b].x * mesh.Tnodes[c].y + mesh.Tnodes[c].x * mesh.Tnodes[a].y - mesh.Tnodes[b].y * mesh.Tnodes[c].x - mesh.Tnodes[c].y * mesh.Tnodes[a].x - mesh.Tnodes[a].y * mesh.Tnodes[b].x;
    double wyz = mesh.Tnodes[a].x * mesh.Tnodes[b].y * mesh.Tnodes[c].z * mesh.Tnodes[b].x * mesh.Tnodes[c].y * mesh.Tnodes[a].z + mesh.Tnodes[c].x * mesh.Tnodes[a].y * mesh.Tnodes[b].z - mesh.Tnodes[a].z * mesh.Tnodes[b].y * mesh.Tnodes[c].x - mesh.Tnodes[b].z * mesh.Tnodes[c].y * mesh.Tnodes[a].x - mesh.Tnodes[c].z * mesh.Tnodes[a].y * mesh.Tnodes[b].x;
    */

    //std::cout << "(" << camera->realN.x << ", " << camera->realN.y << ", " << camera->realN.z << ")\n";

    for(int x = 0; x < img2->width(); x++){
        for(int y = 0; y < img2->height(); y++){
            v = ((x - Xa) * (Yc - Ya) - (y - Ya) * (Xc - Xa))/div;
            w = ((Xb - Xa) * (y - Ya) - (Yb - Ya) * (x - Xa))/div;
            u = 1.0 - v - w;
            if(u < 0 || u > 1 || v < 0 || v > 1 || w < 0 || w > 1) continue;

            double xT = u * texturePoints[0].x() + v * texturePoints[1].x() + w * texturePoints[2].x();
            double yT = u * texturePoints[0].y() + v * texturePoints[1].y() + w * texturePoints[2].y();

            if(yT >= img->height()) yT = img->height() -1;
            if(yT < 0) yT = 0;
            if(xT >= img->width()) xT = img->width() -1;
            if(xT < 0) xT = 0;

            //warunek czy ma byc interpolacja
            /*
            int px = (int)xT;
            int py = (int)yT;
            int p2x = (int)(xT+1);
            int p2y = (int)(yT+1);


            if(p2x >= img->width()){
                p2x = img->width() - 1;
            }
            if(p2y >= img->height()){
                p2y = img->height() - 1;
            }

            if(px < 0) px = 0;
            if(py < 0) py = 0;
            if(p2x < 0) p2x = 0;
            if(p2y < 0) p2y = 0;


            double a = xT - px;
            double b = yT - py;


            QRgb P1 = img->pixel(px,py);
            QRgb P2 = img->pixel(px,p2y);
            QRgb P3 = img->pixel(p2x,p2y);
            QRgb P4 = img->pixel(p2x,py);

            //kolor = b * ((1-a) * P1 + a * P2) + (1-b)*((1-a)*P4 + a * P3);
            kolor.setRed(b * ((1-a) * qRed(P1) + a * qRed(P2)) + (1-b)*((1-a)*qRed(P4) + a * qRed(P3)));
            kolor.setGreen(b * ((1-a) * qGreen(P1) + a * qGreen(P2)) + (1-b)*((1-a)*qGreen(P4) + a * qGreen(P3)));
            kolor.setBlue(b * ((1-a) * qBlue(P1) + a * qBlue(P2)) + (1-b)*((1-a)*qBlue(P4) + a * qBlue(P3)));
            kolor.setAlpha(b * ((1-a) * qAlpha(P1) + a * qAlpha(P2)) + (1-b)*((1-a)*qAlpha(P4) + a * qAlpha(P3)));
            */
            //bez interpolacji

            if(yT >= img->height()) yT = img->height() -1;
            if(yT < 0) yT = 0;
            if(xT >= img->width()) xT = img->width() -1;
            if(xT < 0) xT = 0;

            QRgb rgb = img->pixel(xT, yT);
            kolor = QColor(qRed(rgb), qGreen(rgb), qBlue(rgb), qAlpha(rgb));

            /*
            //Phong Shading czyli liczenie dla kazdego pixela wektora normalnego co daje efekt gladkiej powierzchni
            //*******!!!***********
            double x1, x2, x3;
            double mianownik = (mesh.points[b].y() - mesh.points[a].y());
            if(mianownik == 0.0){
                x1 = mesh.points[a].x();
            }else{
                x1 = ((y - mesh.points[a].y())*(mesh.points[b].x() - mesh.points[a].x()) + (mesh.points[b].y() - mesh.points[a].y()) * x)/mianownik;
            }
            mianownik = (mesh.points[c].y() - mesh.points[b].y());
            if(mianownik == 0.0){
                x2 = mesh.points[b].x();
            }else{
                x2 = ((y - mesh.points[b].y())*(mesh.points[c].x() - mesh.points[b].x()) + (mesh.points[c].y() - mesh.points[b].y()) * x)/mianownik;
            }
            mianownik = (mesh.points[a].y() - mesh.points[c].y());
            if(mianownik == 0.0){
                x3 = mesh.points[c].x();
            }else{
                x3 = ((y - mesh.points[c].y())*(mesh.points[a].x() - mesh.points[c].x()) + (mesh.points[a].y() - mesh.points[c].y()) * x)/mianownik;
            }
            double xMin, xMax, yMin, yMax;
            xMin = minim(mesh.points[a].x(), mesh.points[b].x(), mesh.points[c].x());
            xMax = maxim(mesh.points[a].x(), mesh.points[b].x(), mesh.points[c].x());
            yMin = minim(mesh.points[a].y(), mesh.points[b].y(), mesh.points[c].y());
            yMax = maxim(mesh.points[a].y(), mesh.points[b].y(), mesh.points[c].y());

            Point3D Normal = Na;
            QPoint A = mesh.points[a];
            QPoint B = mesh.points[b];
            QPoint C = mesh.points[c];
            bool test = false;
            if(x1 >= xMin && x1 <= xMax && x2 >= xMin && x2 <= xMax){
                if(x1 <= x2){
                    Normal = phongShading(B,A,C, Nb, Na, Nc, x, y, x1,x2);
                    test = true;
                }else{
                    Normal = phongShading(B,C,A, Nb, Nc, Na, x, y, x2,x1);
                    test = true;
                }
            }else if(x1 >= xMin && x1 <= xMax && x3 >= xMin && x3 <= xMax){
                if(x1 <= x3){
                    Normal = phongShading(A,B,C, Na, Nb, Nc, x, y, x1,x3);
                    test = true;
                }else{
                    Normal = phongShading(A,C,B, Na, Nc, Nb, x, y, x3,x1);
                    test = true;
                }
            }else if(x2 >= xMin && x2 <= xMax && x3 >= xMin && x3 <= xMax){
                if(x2 <= x3){
                    Normal = phongShading(C,B,A, Nc, Nb, Na, x, y, x2,x3);
                    test = true;
                }else{
                    Normal = phongShading(C,A,B, Nc, Na, Nb, x, y, x3,x2);
                    test = true;
                }
            }

            if(test == false){
                //std::cout << x1 << ", "  <<  x2 << ", " <<  x3 << "\n";
                //std::cout << xMin << ", "  <<  xMax << ", " <<  yMin << ", " <<  yMax << "\n";
                //continue;
            }
            Point3D wall = mesh.countNormalVector(mesh.triangles[a],mesh.triangles[b], mesh.triangles[c]);
            Point3D p3d;
            double X;
            double Y;
            double Z;
            Z = (-wall.x * (x - mesh.Tnodes[a].x) + -wall.y * (y - mesh.Tnodes[a].y) + mesh.Tnodes[a].z)/wall.z;
            p3d = Point3D(x,y,Z);
            light = countLightIntense(Normal,p3d);
            */
            //if(light > 0) light = 0;


            int r = (int)(kolor.red() - light);
            int g = (int)(kolor.green() - light);
            int bb = (int)(kolor.blue() - light);

            if(r > 255) r= 255;
            if(r < 0) r = 0;
            if(g > 255) g= 255;
            if(g < 0) g = 0;
            if(bb > 255) bb = 255;
            if(bb < 0) bb = 0;

            setPixel(img2, x, y, r, g, bb);
            /*
            //rzeczy ktory byly probwane do raycasta
            //counting z value of pixel in screen
            //ZBUFOR!!!
            //std::cout << "(" << camera->cX << ", " <<  camera->cY << ", " <<  camera->cZ << ")\n";

            //punkt startowy wektora dla piksela
            Point3D st = camera->realX * (x - 300) + camera->realY * (y - 300);
            st = Point3D(st.x + 500 * -camera->realN.x,st.y + 500 * -camera->realN.y,st.z + 500 * -camera->realN.z);

            double mianow = wall.x * camera->realN.x + wall.y * camera->realN.y + wall.z * camera->realN.z;
            double T;
            if(mianow != 0.0){
                T = (wall.x * st.x - wall.x * mesh.Tnodes[a].x + wall.y * st.y - wall.y * mesh.Tnodes[a].y + wall.z * st.z - wall.z * mesh.Tnodes[a].z)/-mianow;
            }else{
                mianow = 0.1;
                T = (wall.x * st.x - wall.x * mesh.Tnodes[a].x + wall.y * st.y - wall.y * mesh.Tnodes[a].y + wall.z * st.z - wall.z * mesh.Tnodes[a].z)/-mianow;
                //std::cout << "(" << wall.x << ", " << wall.y << ", " << wall.z << ")\n";
                //std::cout << "(" << camera->realN.x << ", " << camera->realN.y << ", " << camera->realN.z << ")\n";
                //continue;
            }
            //punkt przeciecia raya ze sciana
            Point3D crossing = st + (camera->realN * T);
            Point3D crossVec = camera->realN * T;

            //dopisac ogranieczenie ktore bedzie bralo punkty przeciecia w miejscu rzeczywistego wystepowania trojkata a nie gdzies w chuj dalek w innym miejscu
            double strAB = stronaPlaszczyzny(crossing,mesh.Tnodes[a], mesh.Tnodes[b], wall);
            double strBC = stronaPlaszczyzny(crossing,mesh.Tnodes[b], mesh.Tnodes[c], wall);
            double strCA = stronaPlaszczyzny(crossing,mesh.Tnodes[c], mesh.Tnodes[a], wall);


            double odl = sqrt(crossVec.dot(crossVec));
            if(odl < zBufor[x][y] && T > 0 && strAB <= odl/60 && strBC <= odl/60 && strCA <= odl/60){
                zBufor[x][y] = odl;
                //std::cout << "(" << crossing.x << ", " << crossing.y << ", " << crossing.z << ")\n";

                if(x == 300 && y == 300){

                }
            }
            if(x == 270 && y == 296){
                std::cout << mianow << "\n";
                std::cout << "(" << st.x << ", " << st.y << ", " << st.z << ")\n";
                std::cout << "(" << crossing.x << ", " << crossing.y << ", " << crossing.z << ")\n";
                std::cout << "(" << crossVec.x << ", " << crossVec.y << ", " << crossVec.z << ")\n";
                std::cout << "(" << camera->realN.x << ", " << camera->realN.y << ", " << camera->realN.z << ")\n";
                //std::cout << "(" << mesh.Tnodes[a].x << ", " << mesh.Tnodes[b].x << ", " << mesh.Tnodes[c].x << ")\n";
                setPixel(img2, x, y, 255, 0, 0);
                setPixel(img2, x+1, y, 255, 0, 0);
                setPixel(img2, x+1, y+1, 255, 0, 0);
                setPixel(img2, x, y+1, 255, 0, 0);

                std::cout << "StrAB:" << strAB << "\n";
                std::cout << "StrBC:" << strBC << "\n";
                std::cout << "StrCA:" << strCA << "\n";
            }
            */

        }
    }

}

Point3D Painter::phongShading(QPoint A, QPoint B, QPoint C, Point3D Na, Point3D Nb, Point3D Nc, double x, double y, double x1, double x2)
{
    Point3D Nd;
    Point3D Nf;
    Point3D Ne;

    int div = (B.y() - A.y());
    if(div == 0){
        Nd = Na;
    }else{
        Nd = Na * ((B.y() - y)/div) + Nb * ((y - A.y())/div);
    }
    div = C.y() - A.y();
    if(div == 0){
        Nf = Nc;
    }else{
        Nf = Na * ((C.y() - y)/div) + Nc * ((y - A.y())/div);
    }
    div = x2 - x1;
    if(div == 0){
        Ne = Nd;
    }else{
        Ne = Nd * ((x2 - x)/div) + Nf * ((x-x1)/div);
    }
    return Ne;
}

void Painter::drawMesh(Mesh &mesh, QImage *img)
{
    for(int i = 0; i < mesh.triangles.size(); i+=3){
        int idx0 = mesh.triangles[i];
        int idx1 = mesh.triangles[i+1];
        int idx2 = mesh.triangles[i+2];
        QPoint p1 = mesh.points[idx0];
        QPoint p2 = mesh.points[idx1];
        QPoint p3 = mesh.points[idx2];

        Point3D Na = mesh.countNormalVector(idx0, idx1, idx2);

        if(Na.z > 0){
            //drawing triangles mesh
            drawLine(p1.x(), p1.y(), p2.x(), p2.y(), img);
            drawLine(p1.x(), p1.y(), p3.x(), p3.y(), img);
            drawLine(p2.x(), p2.y(), p3.x(), p3.y(), img);

        }
    }
}



void Painter::textureTriangle(std::vector<QPoint> points, std::vector<QPoint> texturePoints, QImage *img, QImage *img2, double light)
{
    double u,v,w;
    QColor kolor;

    double Xa = points[0].x();
    double Xb = points[1].x();
    double Xc = points[2].x();

    double Ya = points[0].y();
    double Yb = points[1].y();
    double Yc = points[2].y();

    double div = (Xb - Xa)  * (Yc - Ya) - (Yb- Ya) * (Xc - Xa);



    for(int x = 0; x < img2->width(); x++){
        for(int y = 0; y < img2->height(); y++){
            v = ((x - Xa) * (Yc - Ya) - (y - Ya) * (Xc - Xa))/div;
            w = ((Xb - Xa) * (y - Ya) - (Yb - Ya) * (x - Xa))/div;
            u = 1.0 - v - w;
            if(u < 0 || u > 1 || v < 0 || v > 1 || w < 0 || w > 1) continue;

            double xT = u * texturePoints[0].x() + v * texturePoints[1].x() + w * texturePoints[2].x();
            double yT = u * texturePoints[0].y() + v * texturePoints[1].y() + w * texturePoints[2].y();

            if(yT >= height) yT = height -1;
            if(yT < 0) yT = 0;
            if(xT >= width) xT = width -1;
            if(xT < 0) xT = 0;

            //warunek czy ma byc interpolacja
            /*
            int px = (int)xT;
            int py = (int)yT;
            int p2x = (int)(xT+1);
            int p2y = (int)(yT+1);


            if(p2x >= img->width()){
                p2x = img->width() - 1;
            }
            if(p2y >= img->height()){
                p2y = img->height() - 1;
            }

            if(px < 0) px = 0;
            if(py < 0) py = 0;
            if(p2x < 0) p2x = 0;
            if(p2y < 0) p2y = 0;


            double a = xT - px;
            double b = yT - py;


            QRgb P1 = img->pixel(px,py);
            QRgb P2 = img->pixel(px,p2y);
            QRgb P3 = img->pixel(p2x,p2y);
            QRgb P4 = img->pixel(p2x,py);

            //kolor = b * ((1-a) * P1 + a * P2) + (1-b)*((1-a)*P4 + a * P3);
            kolor.setRed(b * ((1-a) * qRed(P1) + a * qRed(P2)) + (1-b)*((1-a)*qRed(P4) + a * qRed(P3)));
            kolor.setGreen(b * ((1-a) * qGreen(P1) + a * qGreen(P2)) + (1-b)*((1-a)*qGreen(P4) + a * qGreen(P3)));
            kolor.setBlue(b * ((1-a) * qBlue(P1) + a * qBlue(P2)) + (1-b)*((1-a)*qBlue(P4) + a * qBlue(P3)));
            kolor.setAlpha(b * ((1-a) * qAlpha(P1) + a * qAlpha(P2)) + (1-b)*((1-a)*qAlpha(P4) + a * qAlpha(P3)));
            */
            //bez interpolacji

            if(yT >= height) yT = height -1;
            if(yT < 0) yT = 0;
            if(xT >= width) xT = width -1;
            if(xT < 0) xT = 0;
            QRgb rgb = img->pixel(xT, yT);
            kolor = QColor(qRed(rgb), qGreen(rgb), qBlue(rgb), qAlpha(rgb));
            int r = (int)(kolor.red() + light);
            int g = (int)(kolor.green() + light);
            int b = (int)(kolor.blue() + light);

            if(r > 255) r= 255;
            if(r < 0) r = 0;
            if(g > 255) g= 255;
            if(g < 0) g = 0;
            if(b > 255) b= 255;
            if(b < 0) b = 0;

            setPixel(img2, x, y, r, g, b);

        }
    }

}


void Painter::drawLine(int x0, int y0, int x1, int y1, QImage *img)
{
    float a, b;

    //if(x0 < 0 || x0 > width || x1 < 0 || x1 > width) return;
    //if(y0 < 0 || y0 > height || y1 < 0 || y1 > height) return;

    if(x0 < 0) x0 = 0;
    if(x0 >= width) x0 = width-1;

    if(x1 < 0) x1 = 0;
    if(x1 >= width) x1 = width-1;

    if(y0 < 0) y0 = 0;
    if(y0 >= width) y0 = height-1;

    if(y1 < 0) y1 = 0;
    if(y1 >= width) y1 = height-1;

    if(x0 == x1 ){
        a = 0;
        b = y1;
    }else if(y0 == y1){
        b = y1;
        a = 0;
    }else{
        a = (float)(y1 - y0)/(x1 -x0);
        b = (float)(y0 - a * x0);
    }
    if(x0 < x1){
        if(abs(a) < 1){
            for(int x = x0; x <=x1; x++){
                int y = a * x + b;
                y = (int)floor(y + 0.5f);

                setPixel(img, x, y, 0,0,0);
            }
        }else if(abs(a) >= 1 && y0 <= y1 ){
            for(int y = y0; y <=y1; y++){
                int x = (y - b)/a;
                x = (int)floor(x + 0.5f);

                setPixel(img, x, y, 0,0,0);
            }
        }else{
            for(int y = y1; y <=y0; y++){
                int x = (y - b)/a;
                x = (int)floor(x + 0.5f);

                setPixel(img, x, y, 0,0,0);
            }
        }


    }else if(x0 > x1){
        if(abs(a) < 1){
            for(int x = x1; x <=x0; x++){
                int y = a * x + b;
                y = (int)floor(y + 0.5f);

                setPixel(img, x, y, 0,0,0);
            }
        }else if(abs(a) >= 1 && y1 <= y0){
            for(int y = y1; y <=y0; y++){
                int x = (y - b)/a;
                x = (int)floor(x + 0.5f);

                setPixel(img, x, y, 0,0,0);
            }

        }else{
            for(int y = y0; y <=y1; y++){
                int x = (y - b)/a;
                x = (int)floor(x + 0.5f);

                setPixel(img, x, y, 0,0,0);
            }
        }
    }else if(x0 == x1 && y0 < y1){
        for(int y = y0; y <=y1; y++){
            int x;
            x= x0;
            setPixel(img, x, y, 0,0,0);
        }
    }else{
        for(int y = y1; y <=y0; y++){
            int x;

            x= x0;
            setPixel(img, x, y, 0,0,0);
        }
    }
}
void Painter::setPixel(QImage *img, int x, int y, short red, short green, short blue)
{
    if(x > img->width() || x < 0 || y > img->height() || y < 0) return;
    unsigned char* ptr = img->bits();

    ptr[4 * img->width() * y + 4 * x] = blue;
    ptr[4 * img->width() * y + 4 * x + 1] = green;
    ptr[4 * img->width() * y + 4 * x + 2] = red;
    ptr[4 * img->width() * y + 4 * x + 3] = 255;

}
