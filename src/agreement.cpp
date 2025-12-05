#include "agreement.h"
#include "ui_agreement.h"

Agreement::Agreement(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Agreement)
{
    ui->setupUi(this);
}

Agreement::~Agreement()
{
    delete ui;
}

void Agreement::on_Return_clicked()
{
    accept();
}
