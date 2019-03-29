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
