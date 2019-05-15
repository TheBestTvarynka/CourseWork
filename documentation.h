#ifndef DOCUMENTATION_H
#define DOCUMENTATION_H

#include <QMainWindow>

namespace Ui {
class Documentation;
}

class Documentation : public QMainWindow
{
    Q_OBJECT

public:
    explicit Documentation(QWidget *parent = 0);
    ~Documentation();

private:
    Ui::Documentation *ui;
};

#endif // DOCUMENTATION_H
