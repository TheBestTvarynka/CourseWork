#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->comboBox->addItem("Queen");
//    ui->comboBox->addItem("King");
//    ui->comboBox->addItem("Rook");
//    ui->comboBox->addItem("Knight");
//    ui->comboBox->addItem("Bishop");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->widget->type_of_new_figure = index;
}

void MainWindow::on_actionSave_triggered()
{
    //
}

void MainWindow::on_actionQuit_triggered()
{
    QCoreApplication::quit();
}
