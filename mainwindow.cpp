#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    file_name = nullptr;
    unsaved_work = false;
    path = false;

    connect(ui->widget, SIGNAL(check_fights(int)), this, SLOT(on_check_clicked(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::write_file()
{
    setWindowTitle("[" + file_name + "] - Chess program");
    QVector<figure> *data = ui->widget->GetData();
    // qDebug() << data->size() << " " << file_name;

    QFile Output(file_name);
    if (!Output.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "Error: can not open file...";
    QTextStream out(&Output);
    for (int i = 0; i < data->size(); i++)
    {
        out << id_to_string((*data)[i].type) << " " << (*data)[i].x << " " << (*data)[i].y << "\n";
        // qDebug() << id_to_string((*data)[i].type) << " " << (*data)[i].x << " " << (*data)[i].y << "\n";
    }
    Output.flush();
    Output.close();
}

void MainWindow::read_file()
{
    figure tmp;
    QString buff;
    QVector<figure> *data = ui->widget->GetData();

    QFile Input(file_name);
    if (!Input.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "Error: can not open file...";
    QTextStream in(&Input);

    data->clear();

    in >> buff;
    tmp.type = string_to_id(buff);
    in >> tmp.x;
    in >> tmp.y;
    while (!in.atEnd())
    {
        // qDebug() << tmp.type << " " << tmp.x << " " << tmp.y << "\n";
        data->push_back(tmp);
        in >> buff;
        tmp.type = string_to_id(buff);
        in >> tmp.x;
        in >> tmp.y;
    }

    Input.flush();
    Input.close();
}

int MainWindow::string_to_id(QString name)
{
    if (name == "king")
        return 0;
    if (name == "queen")
        return 1;
    if (name == "rook")
        return 2;
    if (name == "bishop")
        return 3;
    if (name == "knight")
        return 4;
    return 0;
}

QString MainWindow::id_to_string(int id)
{
    switch (id)
    {
    case 0:
        return "king";
    case 1:
        return "queen";
    case 2:
        return "rook";
    case 3:
        return "bishop";
    case 4:
        return "knight";
    default:
        return nullptr;
    }
    return "";
}

void MainWindow::save_work()
{
    if (unsaved_work)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Save?", "You have unsaved work. Save it?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            if (file_name == nullptr)
                file_name = QFileDialog::getSaveFileName(this, tr("Save file"), "/home/", "All files (*.*);;Text file (*.txt);;CSV-file (*.csv)");
            write_file();
        }
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->widget->type_of_new_figure = index;
}

void MainWindow::on_actionSave_triggered()
{
    if (file_name == nullptr)
        file_name = QFileDialog::getSaveFileName(this, tr("Save file"), "/home/", "All files (*.*);;Text file (*.txt);;CSV-file (*.csv)");
    if (file_name != nullptr)
        write_file();
}

void MainWindow::on_actionQuit_triggered()
{
    save_work();
    QCoreApplication::quit();
}

void MainWindow::on_actionOpen_triggered()
{
    save_work();
    file_name = QFileDialog::getOpenFileName(this, tr("Open file"), "/home/", "All files (*.*);;Text file (*.txt);;CSV-file (*.csv)");
    if (file_name != nullptr)
    {
        // QMessageBox::information(this, tr("file name"), file_name);
        setWindowTitle("[*" + file_name + "] - Chess program");
    }
    else
    {
        QMessageBox::information(this, tr("file name"), "Error: file not open...");
        return;
    }
    unsaved_work = true;
    read_file();
}

void MainWindow::on_actionNew_triggered()
{
    save_work();
    setWindowTitle("[*Untitled.txt] - Chess program");
    unsaved_work = true;
    file_name = nullptr;
}

void MainWindow::on_actionSave_as_triggered()
{
    file_name = QFileDialog::getSaveFileName(this, tr("Save file as"), "/home/", "All files (*.*);;Text file (*.txt);;CSV-file (*.csv)");
    write_file();
}

void MainWindow::on_pushButton_4_clicked()
{
    int s = ui->widget->selectedFigure;
    if (s != -1)
    {
        QVector<figure> *data = ui->widget->GetData();
        data->erase(data->begin() + s);
        ui->widget->selectedFigure = -1;
    }
}

void MainWindow::on_clear_scene_clicked()
{
    QVector<figure> *data = ui->widget->GetData();
    data->clear();
    QVector<point> *battle = ui->widget->GetBattle();
    battle->clear();
    ui->widget->selectedFigure = -1;
}

void MainWindow::on_change_type_currentIndexChanged(int index)
{
    QVector<figure> *data = ui->widget->GetData();
    int s = ui->widget->selectedFigure;
    (*data)[s].type = index;
}

void MainWindow::on_check_clicked(int selected)
{
    qDebug() << "Yes, I find solution of my problem... " << selected;
//     we search pray for selected figure
//    bool insert = false;
    QVector<figure> *data = ui->widget->GetData();
    QVector<point> *battle = ui->widget->GetBattle();
    battle->clear();
    point tmp;
    float d1, d2;

    for (int i = 0; i < data->size(); i++)
    {
        if (i == selected)
            continue;
        // qDebug() << "compare: " << (*data)[i].x << " " << (*data)[selected].x;
        if ((*data)[i].y == (*data)[selected].y)
        {
            d1 = (*data)[i].x - (*data)[selected].x;
            // now we need pass throught the battle[j] and check all
            for (int j = 0; j < battle->size(); j++)
            {
                d2 = (*battle)[j].x - (*data)[selected].x;
                // if we need to erase pray
                qDebug() << "distances: " << d1 << " " << d2;
                if (d1 * d2 > 0)
                {
                    if (abs(d1) < abs(d2))
                    {
                        qDebug() << "erase " << (*battle)[j].x << " " << (*battle)[j].y;
                        battle->erase(battle->begin() + j);
                    }
                    else
                        return;
                }
            }
            tmp.x = (*data)[i].x;
            tmp.y = (*data)[i].y;
            battle->push_back(tmp);
            qDebug() << "add " << tmp.x << " " << tmp.y;
        }
        if ((*data)[i].x == (*data)[selected].x)
        {
            d1 = (*data)[i].y - (*data)[selected].y;
            // now we need pass throught the battle[j] and check all
            for (int j = 0; j < battle->size(); j++)
            {
                d2 = (*battle)[j].y - (*data)[selected].y;
                // if we need to erase pray
                qDebug() << "distances: " << d1 << " " << d2;
                if (d1 * d2 > 0)
                {
                    if (abs(d1) < abs(d2))
                    {
                        qDebug() << "erase " << (*battle)[j].x << " " << (*battle)[j].y;
                        battle->erase(battle->begin() + j);
                    }
                    else
                        return;
                }
            }
            tmp.x = (*data)[i].x;
            tmp.y = (*data)[i].y;
            battle->push_back(tmp);
            qDebug() << "add " << tmp.x << " " << tmp.y;
        }
    }
}
