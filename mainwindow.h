#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <painter.h>
#include <sphere.h>
#include <scene.h>
#include <QTimer>
#include <QTimerEvent>

#include <QMainWindow>
#include <QKeyEvent>
#include <QPainter>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QImage *MainImage;
    QImage *TextureImage;

    int width, height;
    int startX, startY;

    //Painter which paints pixels
    Painter painter;
    //Scene which contains all 3d objects
    Scene scene;

    //object which represents mesh
    //Mesh which is root tree of all meshes
    Mesh root;
    Mesh mesh;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent*event);

    void timerEvent(QTimerEvent *event);
    int timerID;
    int time;   //zmienna zwiekszana za kazdym wywoalniem funkcji timerEvent
    //zmienna ktora gdy jest aktywna kamera sledzi ruch ziemi
    bool earthCam;

private slots:
    void paintEvent(QPaintEvent*);

    void on_PrzesunX_valueChanged(int value);
    void on_PrzesunY_valueChanged(int value);
    void on_PrzesunZ_destroyed();
    void on_PrzesunZ_valueChanged(int value);
    void on_SkalX_valueChanged(int value);
    void on_SkalY_valueChanged(int value);
    void on_SkalZ_valueChanged(int value);
    void on_RotX_valueChanged(int value);
    void on_RotY_valueChanged(int value);
    void on_RotZ_valueChanged(int value);
    void on_ShrY_valueChanged(int value);
    void on_ShrX_valueChanged(int value);
    void on_ShrZ_valueChanged(int value);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_toggled(bool checked);
    void on_pushButton_4_toggled(bool checked);
    void on_pushButton_5_toggled(bool checked);
    void on_pushButton_6_toggled(bool checked);
};

#endif // MAINWINDOW_H
