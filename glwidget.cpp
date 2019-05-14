#include "glwidget.h"
#include <math.h>
#include <QDebug>
#include <QColor>

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
    show_path = true;
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

	for (int i = 0; i < 6; i++)
	{
		if (!img.load(paths[i]))
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
	SizeSquare = float(SizeChessboard) / float(8);
}

void GLWidget::paintGL()
{
	QColor background(50, 52, 61, 255);
	qglClearColor(background);
//    qglClearColor(Qt::gray);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable (GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	// enable alfa
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw chessboard
	qglColor(Qt::white);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f (0.0f, 0.0f);    glVertex2i(0, 0);
	glTexCoord2f (0.0f, 1.0f);    glVertex2i(SizeChessboard, 0);
	glTexCoord2f (1.0f, 1.0f);    glVertex2i(SizeChessboard, SizeChessboard);
	glTexCoord2f (1.0f, 0.0f);    glVertex2i(0, SizeChessboard);
	glEnd();

	// draw figures
	if (selectedFigure != -1)
	{
		// qDebug() << selectedFigure << endl;
        qglColor(Qt::green);
		glLineWidth(5);
		glBegin(GL_LINE_LOOP);
		glVertex2i(SizeSquare * (figures[selectedFigure].x - 1), SizeSquare * (figures[selectedFigure].y - 1));
		glVertex2i(SizeSquare * figures[selectedFigure].x, SizeSquare * (figures[selectedFigure].y - 1));
		glVertex2i(SizeSquare * figures[selectedFigure].x, SizeSquare * figures[selectedFigure].y);
		glVertex2i(SizeSquare * (figures[selectedFigure].x - 1), SizeSquare * figures[selectedFigure].y);
		glEnd();
	}
    if (show_path)
    {
        for (int p = 0; p < battle.size(); p++)
        {
            glLineWidth(20);
            qglColor(Qt::red);
            glBegin(GL_LINES);
            glVertex2i(SizeSquare * (battle[p].x - 0.5), SizeSquare * (battle[p].y - 0.5));
            glVertex2i(SizeSquare * (figures[selectedFigure].x - 0.5), SizeSquare * (figures[selectedFigure].y - 0.5));
            glEnd();
        }
    }
	for (int i = 0; i < figures.size(); i++)
	{
		qglColor(Qt::white);
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
    show_path = false;
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
		if (figures.empty())
			selectedFigure = -1;
		else
			selectObject();
		// qDebug() << selectedFigure << " ";
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *ap)
{
	float step = SizeSquare;
	if (ap->buttons() == Qt::LeftButton && selectedFigure != -1)
	{
		// qDebug() << selectedFigure << " - ";
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

    int delta = 1;
    int X, Y, locX, locY;
    mouseDeltaX = 0;
    mouseDeltaY = 0;
    show_path = true;

    if (ap->buttons() == Qt::RightButton || selectedFigure == -1)
        return;

	X = figures[selectedFigure].x = myCeil(figures[selectedFigure].x);
	Y = figures[selectedFigure].y = myCeil(figures[selectedFigure].y);
	if (overlays(X, Y))
	{
		// that means we have overlays
		while (delta <= 8)
		{
			locX = std::max(1, X - delta);
			locY = std::max(1, Y - delta);
			for (int a = locX; a <= std::min(8, X + delta); a++)
			{
				if (!overlays(a, locY))
				{
					figures[selectedFigure].x = a;
					figures[selectedFigure].y = locY;
                    emit check_fights(selectedFigure);
					return;
				}
			}
			locX = std::min(8, X + delta);
			for (int b = locY; b <= std::min(8, Y + delta); b++)
			{
				if (!overlays(locX, b))
				{
					figures[selectedFigure].x = locX;
					figures[selectedFigure].y = b;
                    emit check_fights(selectedFigure);
                    return;
				}
			}
			locY = std::min(8, Y + delta);
			for (int c = locX; c >= std::max(1, X - delta); c--)
			{
				if (!overlays(c, locY))
				{
					figures[selectedFigure].x = c;
					figures[selectedFigure].y = locY;
                    emit check_fights(selectedFigure);
					return;
				}
			}
			locX = std::max(1, X - delta);
			for (int d = locY; d >= std::max(1, Y - delta); d--)
			{
				if (!overlays(locX, d))
				{
					figures[selectedFigure].x = locX;
					figures[selectedFigure].y = d;
                    emit check_fights(selectedFigure);
					return;
				}
			}
			delta++;
		}
	}
    else
    {
        emit check_fights(selectedFigure);
        emit update_list();
    }
}

void GLWidget::selectObject()
{
	if (figures.size() == 0)
		return;
	float minDistanse = pow(mousePosX - (figures[0].x - 0.5) * SizeSquare, 2) + pow(mousePosY - (figures[0].y - 0.5) * SizeSquare, 2);
	float distanse;
	selectedFigure = 0;
	for (int i = 0; i < figures.size(); i++)
	{
		distanse = pow(mousePosX - (figures[i].x - 0.5) * SizeSquare, 2) + pow(mousePosY - (figures[i].y - 0.5) * SizeSquare, 2);
		if (distanse < minDistanse)
		{
			minDistanse = distanse;
			selectedFigure = i;
		}
	}
}

bool GLWidget::overlays(int x, int y)
{
	for (int j = 0; j < figures.size(); j++)
	{
		if (j != selectedFigure && figures[j].x == x && figures[j].y == y)
			return true;
	}
	return false;
}

QVector<figure> *GLWidget::GetData()
{
    return &figures;
}

QVector<point> *GLWidget::GetBattle()
{
    return &battle;

}

void GLWidget::SetShow_path(bool clicked)
{
    show_path = clicked;
}

int GLWidget::myCeil(float i)
{
	if (i - (int)(i) >= 0.5)
		i = (int)(i) + 1;
	if ((int)(i) <= 0)
		i = 1.0;
	if (i >= 9)
		i = 8;
	return (int)(i);
}
