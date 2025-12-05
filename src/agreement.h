#ifndef AGREEMENT_H
#define AGREEMENT_H

#include <QDialog>

namespace Ui {
class Agreement;
}

class Agreement : public QDialog
{
    Q_OBJECT

public:
    explicit Agreement(QWidget *parent = nullptr);
    ~Agreement();

private slots:
    void on_Return_clicked();

private:
    Ui::Agreement *ui;
};

#endif // AGREEMENT_H
