#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    width = ui->frame->width();
    height = ui->frame->height();

    startX = ui->frame->x();
    startY = ui->frame->y();



    MainImage = new QImage(width, height, QImage::Format_ARGB32);
    MainImage->fill(QColor(0,0,0,255));

    TextureImage = new QImage(width, height, QImage::Format_ARGB32);
    TextureImage->fill(QColor(106,107,74,255));
    TextureImage->load(":/images/texture.jpg");
    *TextureImage = TextureImage->scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::FastTransformation);


    scene = Scene(&painter,MainImage);
    painter = Painter(width,height,scene.getCamera());

    root = Mesh();
    //mesh = Sphere(TextureImage,3,3,0,0,0, 10);
    //creating floor
    mesh = Mesh(&root,TextureImage, 5, 0, 5, 0);


    scene.Objects.push_back(&mesh);
    scene.generateWorld(root, TextureImage);

    timerID = startTimer(1000);
    time = 1;
    earthCam = false;
    update();
}

MainWindow::~MainWindow()
{
    killTimer(timerID);
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W){
        scene.getCamera()->cX+=3.0 * scene.getCamera()->realN.x;
        scene.getCamera()->cY+=3.0 * scene.getCamera()->nY;
        scene.getCamera()->cZ+=3.0 * scene.getCamera()->realN.z;
    }else if(event->key() == Qt::Key_S){
        scene.getCamera()->cX-=3 * scene.getCamera()->realN.x;
        scene.getCamera()->cY-=3 * scene.getCamera()->nY;
        scene.getCamera()->cZ-=3 * scene.getCamera()->realN.z;
    }else if(event->key() == Qt::Key_D){
        scene.getCamera()->cX-=3 * -scene.getCamera()->wallVec.x;
        scene.getCamera()->cY-=3 * scene.getCamera()->wallVec.y;
        scene.getCamera()->cZ-=3 * scene.getCamera()->wallVec.z;
    }else if(event->key() == Qt::Key_A){
        scene.getCamera()->cX+=3 * -scene.getCamera()->wallVec.x;
        scene.getCamera()->cY+=3 * scene.getCamera()->wallVec.y;
        scene.getCamera()->cZ+=3 * scene.getCamera()->wallVec.z;
    }else if(event->key() == Qt::Key_E){
        scene.getCamera()->tY-=3;
    }else if(event->key() == Qt::Key_Q){
        scene.getCamera()->tY+=3;
    }else if(event->key() == Qt::Key_I){
        scene.getCamera()->tX-=3;
    }else if(event->key() == Qt::Key_K){
        scene.getCamera()->tX+=3;
    }
    scene.refresh();
    update();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    double rok = 1;
    double dzien = 1;
    //sun
    scene.sun->rotateY(-2.1 * time);
    //mercury
    scene.mercury->rotateY(1 * time);
    scene.planetRound(0,rok * 4,0.1,scene.mercury,scene.mercPos);
    //venus
    scene.venus->rotateY(-0.3 * time);
    scene.planetRound(0.1,1.8 * rok,0,scene.venus,scene.venPos);
    //earth
    scene.planetRound(0,rok,0,scene.earth, scene.earthPos);
    scene.earth->rotateY(dzien * time);
    scene.refresh();
    //mars
    scene.mars->rotateY(0.9 * time);
    scene.planetRound(0.2,1.8 * rok,0.5,scene.mars,scene.marsPos);
    //jowisz
    scene.jowisz->rotateY(3 * dzien * time);
    scene.planetRound(0.0,rok * 0.09,0.0,scene.jowisz,scene.jowPos);
    //saturn
    scene.saturn->rotateY(3 * dzien * time);
    scene.planetRound(0,rok * 0.03,0,scene.saturn,scene.satPos);
    //uran
    scene.uran->rotateY(1.8 * dzien * time);
    scene.planetRound(0,rok * 0.02, 0, scene.uran, scene.uraPos);
    //neptun
    scene.neptun->rotateY(1.5 * dzien * time);
    scene.planetRound(0,rok * 0.01, 0, scene.neptun, scene.nepPos);

    //tryb gdy kamera prusza sie wraz z ziemia
    if(earthCam){
        scene.getCamera()->cX = scene.earthPos.x;
        scene.getCamera()->cY = scene.earthPos.y;
        scene.getCamera()->cZ = scene.earthPos.z - 100;
    }
    update();
    time++;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    //p.drawImage(startX, startY, *img);
    p.drawImage(startX, startY, *MainImage);

}


void MainWindow::on_PrzesunX_valueChanged(int value)
{
    mesh.translateX(value);
    //scene.getCamera()->cX = value;
    scene.refresh();
    update();
}

void MainWindow::on_PrzesunY_valueChanged(int value)
{
    mesh.translateY(value);
    //scene.getCamera()->cY = value;
    scene.refresh();
    update();
}



void MainWindow::on_PrzesunZ_valueChanged(int value)
{
    mesh.translateZ(value);
    //scene.getCamera()->cZ = value;
    scene.refresh();
    update();
}

void MainWindow::on_SkalX_valueChanged(int value)
{
    mesh.scaleX(value/50.0);
    scene.refresh();
    update();
}

void MainWindow::on_SkalY_valueChanged(int value)
{
    mesh.scaleY(value/50.0);
    scene.refresh();
    update();
}

void MainWindow::on_SkalZ_valueChanged(int value)
{
    mesh.scaleZ(value/50.0);
    scene.refresh();
    update();
}

void MainWindow::on_RotX_valueChanged(int value)
{
    //mesh.rotateX(value);
    scene.getCamera()->tX = value;
    scene.refresh();
    update();
}

void MainWindow::on_RotY_valueChanged(int value)
{
    mesh.rotateY(value);
    //scene.getCamera()->tY = value;
    scene.refresh();
    update();
}

void MainWindow::on_RotZ_valueChanged(int value)
{
    //mesh.rotateZ(value);
    scene.getCamera()->tZ = value;
    scene.refresh();
    update();
}

void MainWindow::on_ShrX_valueChanged(int value)
{
    mesh.shearingX(value/50.0);
    scene.refresh();
    update();
}

void MainWindow::on_ShrY_valueChanged(int value)
{
    mesh.shearingY(value/50.0);
    scene.refresh();
    update();
}



void MainWindow::on_ShrZ_valueChanged(int value)
{
    mesh.shearingZ(value/50.0);
    scene.refresh();
    update();
}

void MainWindow::on_pushButton_clicked()
{
    scene.planetRound(0,rand(), 0, scene.mercury, scene.mercPos);
    scene.planetRound(0,rand(), 0, scene.venus, scene.venPos);
    scene.planetRound(0,rand(), 0, scene.earth, scene.earthPos);
    scene.planetRound(0,rand(), 0, scene.mars, scene.marsPos);
    scene.planetRound(0,rand(), 0, scene.jowisz, scene.jowPos);
    scene.planetRound(0,rand(), 0, scene.saturn, scene.satPos);
    scene.planetRound(0,rand(), 0, scene.uran, scene.uraPos);
    scene.planetRound(0,rand(), 0, scene.neptun, scene.nepPos);
    scene.refresh();
    update();

}

void MainWindow::on_pushButton_2_clicked()
{
    close();
}

void MainWindow::on_PrzesunZ_destroyed()
{

}

void MainWindow::on_pushButton_3_toggled(bool checked)
{
    if(checked){
        scene.getCamera()->cX = 0;
        scene.getCamera()->cY = -1000;
        scene.getCamera()->cZ = 2500;

        scene.getCamera()->tX = 15;
        scene.getCamera()->tY = 0;
        scene.getCamera()->tZ = 0;
        scene.getCamera()->clear();
        scene.getCamera()->refresh();

    }
}

void MainWindow::on_pushButton_4_toggled(bool checked)
{
    if(checked){
        scene.getCamera()->cX = 0;
        scene.getCamera()->cY = 0;
        scene.getCamera()->cZ = 0;

        scene.getCamera()->tX = 0;
        scene.getCamera()->tY = 0;
        scene.getCamera()->tZ = 0;
        scene.getCamera()->clear();
        scene.getCamera()->refresh();
    }
}

void MainWindow::on_pushButton_5_toggled(bool checked)
{
    if(checked){
        scene.getCamera()->cX = 0;
        scene.getCamera()->cY = 0;
        scene.getCamera()->cZ = 0;

        scene.getCamera()->tX = 0;
        scene.getCamera()->tY = 180;
        scene.getCamera()->tZ = 0;
        scene.getCamera()->clear();
        scene.getCamera()->refresh();
    }
}

void MainWindow::on_pushButton_6_toggled(bool checked)
{
    if(checked){
        scene.getCamera()->cX = scene.earthPos.x;
        scene.getCamera()->cY = scene.earthPos.y;
        scene.getCamera()->cZ = scene.earthPos.z - 100;

        scene.getCamera()->tX = 0;
        scene.getCamera()->tY = 180;
        scene.getCamera()->tZ = 0;
        scene.getCamera()->clear();
        scene.getCamera()->refresh();
    }
    earthCam = checked;
}
