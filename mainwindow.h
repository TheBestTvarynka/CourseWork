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
    Ui::MainWindow *ui;
    bool unsaved_work;
    bool path;
    QVector<std::pair<float, float> > path_s;
    QString file_name;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void write_file();
    void read_file();
    void save_work();
    int string_to_id(QString name);
    QString id_to_string(int id);

    void check_rook(int);
    void check_bishop(int);
    void check_knight(int);
    void check_king(int);
//    void check_queen()
private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_actionSave_triggered();
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionSave_as_triggered();
    void on_pushButton_4_clicked();
    void on_clear_scene_clicked();
    void on_change_type_currentIndexChanged(int index);
    void on_check_clicked(int);

    void on_hide_paths_clicked(bool checked);
};

#endif // MAINWINDOW_H
