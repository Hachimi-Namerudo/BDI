#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Question.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void beck();
    void pull(int index);
    void push(int index);
    void calculate();


private slots:
    void on_Agreement_clicked();

    void on_ins_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_Enter_clicked();

    void on_prePage_clicked();

    void on_nextPage_clicked();

    void on_submit_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Question*> questions;
    int score = 0;
    int currentPage;
};
#endif // MAINWINDOW_H

