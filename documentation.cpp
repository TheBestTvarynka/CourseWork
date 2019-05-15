#include "documentation.h"
#include "ui_documentation.h"

Documentation::Documentation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Documentation)
{
    ui->setupUi(this);
}

Documentation::~Documentation()
{
    delete ui;
}
