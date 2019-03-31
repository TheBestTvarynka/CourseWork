#include "glwidget.h"
#include <math.h>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent): QGLWidget(parent)
{
    // add one figure on board
    figure tmp;
    tmp.x = 4.0;
    tmp.y = 2.0;
    // type is index if texture[] array; in this situation, 0 - texture of king
    tmp.type = 4;
    figures.push_back(tmp);
    selectedFigure = -1;
    // set a timer
    connect( &mpTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    mpTimer.start(100);

    SizeSquare = 649;
    type_of_new_figure = 0;
    texture = new GLuint[6];
    mouseDeltaX = 0;
    mouseDeltaY = 0;

}

void GLWidget::initializeGL()
{
    QString paths[6];
    paths[0] = ":/rextures/textures/kingWhite.png";
    paths[1] = ":/rextures/textures/queenWhite.png";
    paths[2] = ":/rextures/textures/rookWhite.png";
    paths[3] = ":/rextures/textures/bishopWhite.png";
    paths[4] = ":/rextures/textures/knightWhite.png";
    paths[5] = ":/rextures/textures/5.png";

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 649, 649, 0, 0, 1);
    SizeSquare = 649 / 8;
    glEnable (GL_TEXTURE_2D);
    ////////// enable alfa
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // take special id for our texture
    glGenTextures(6, texture);

    for(int i = 0; i < 6; i++)
    {
        if(!img.load(paths[i]))
            qDebug() << "Error: can not load the image \n";
        GL_formatted_image = QGLWidget::convertToGLFormat(img);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        // sets various perameters for current texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // load texture into RAM
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GL_formatted_image.width(), GL_formatted_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image.bits());
    }
}

void GLWidget::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0, 1);
    glViewport(0, 0, w, h);
    SizeChessboard = std::min(w, h);
    SizeSquare = SizeChessboard / 8;
}

void GLWidget::paintGL()
{
    qglClearColor(Qt::gray);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable (GL_TEXTURE_2D);
    // enable alfa
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw chessboard
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBegin(GL_QUADS);
    glTexCoord2f (0.0f, 0.0f);    glVertex2i(0, 0);
    glTexCoord2f (0.0f, 1.0f);    glVertex2i(SizeChessboard, 0);
    glTexCoord2f (1.0f, 1.0f);    glVertex2i(SizeChessboard, SizeChessboard);
    glTexCoord2f (1.0f, 0.0f);    glVertex2i(0, SizeChessboard);
    glEnd();

    // draw figures
    for(int i = 0; i < figures.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, texture[figures[i].type]);
        glBegin(GL_QUADS);
        glTexCoord2f (0.0f, 0.0f);    glVertex2i(SizeSquare * (figures[i].x - 1), SizeSquare * (figures[i].y - 1));
        glTexCoord2f (1.0f, 0.0f);    glVertex2i(SizeSquare * figures[i].x, SizeSquare * (figures[i].y - 1));
        glTexCoord2f (1.0f, 1.0f);    glVertex2i(SizeSquare * figures[i].x, SizeSquare * figures[i].y);
        glTexCoord2f (0.0f, 1.0f);    glVertex2i(SizeSquare * (figures[i].x - 1), SizeSquare * figures[i].y);
        glEnd();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *ap)
{
    figure tmp;
    int countP;
    if (ap->buttons() == Qt::RightButton)
    {
        countP = ap->x() / SizeSquare;
        tmp.x = countP + 1;
        countP = ap->y() / SizeSquare;
        tmp.y = countP + 1;
        tmp.type = type_of_new_figure;
        figures.push_back(tmp);
        selectedFigure = figures.size() - 1;
    }
    if (ap->buttons() == Qt::LeftButton)
    {
        mousePosX = ap->x();
        mousePosY = ap->y();
        selectObject();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *ap)
{
    float step = SizeSquare;
    if (ap->buttons() == Qt::LeftButton && selectedFigure != -1)
    {
        mouseDeltaX = ap->x() - mousePosX;
        mouseDeltaY = ap->y() - mousePosY;
        mousePosX = ap->x();
        mousePosY = ap->y();
        figures[selectedFigure].x += mouseDeltaX / step;
        figures[selectedFigure].y += mouseDeltaY / step;
        mouseDeltaX = 0;
        mouseDeltaY = 0;
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *ap)
{
    mouseDeltaX = 0;
    mouseDeltaY = 0;

    figures[selectedFigure].x = myCeil(figures[selectedFigure].x);
    figures[selectedFigure].y = myCeil(figures[selectedFigure].y);
}

void GLWidget::selectObject()
{
    if (figures.size() == 0)
        return;
    float minDistanse = pow(mousePosX - (figures[0].x - 0.5)*SizeSquare, 2) + pow(mousePosY - (figures[0].y - 0.5)*SizeSquare, 2);
    float distanse;
    selectedFigure = 0;
    for (int i = 0; i < figures.size(); i++)
    {
        distanse = pow(mousePosX - (figures[i].x - 0.5)*SizeSquare, 2) + pow(mousePosY - (figures[i].y - 0.5)*SizeSquare, 2);
        if (distanse < minDistanse)
        {
            minDistanse = distanse;
            selectedFigure = i;
        }
    }
}

QVector<figure> *GLWidget::GetData()
{
    return &figures;
}

int GLWidget::myCeil(float i)
{
    if (i - (int)(i) >= 0.5)
        return (int)(i) + 1;

    return (int)(i);
}
