#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QVector>
#include "data_structure.cpp"

class GLWidget : public QGLWidget
{
    Q_OBJECT
    QVector<point> battle;
    QVector<figure> figures;
    QTimer mpTimer;
    //////////////// for textures
    QImage img;
    GLuint *texture;
    QImage GL_formatted_image;
    ////////////
    int mouseDeltaX;
    int mouseDeltaY;
    int mousePosX;
    int mousePosY;
    float SizeSquare;
    int SizeChessboard;
    bool show_path;
public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    ////////////
    void selectObject();
    bool overlays(int, int);
    QVector<figure> *GetData();
    QVector<point> *GetBattle();
    void SetShow_path(bool);
    int myCeil(float);
    ///////////
    int type_of_new_figure;
    int selectedFigure;
signals:
    void check_fights(int);
};

#endif // GLWIDGET_H
