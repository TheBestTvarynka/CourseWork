#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVector>
// #include "data_structure.cpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void write_file();
    void read_file();
    int string_to_id(QString name);
    QString id_to_string(int id);
    void save_work();
private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_actionSave_triggered();
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
    void on_actionNew_triggered();

    void on_actionSave_as_triggered();

private:
    Ui::MainWindow *ui;

    bool unsaved_work;
    QString file_name;
    // QVector<figure> *data;
};

#endif // MAINWINDOW_H
