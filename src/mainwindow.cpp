#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "agreement.h"
#include "instruction.h"
#include <QFile>
#include "question.h"
#include <QMessageBox>
#include <qradiobutton.h>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("贝克抑郁量表测评入口");
    ui->Enter->setEnabled(0);
    ui->checkBox->setEnabled(0);
    ui->version->setText("版本：1.0.01 Beta");

    ui->submit->hide();
    currentPage = 1;
    ui->pageHint->setText((QString("当前页码：1 / 3")));

    ui->mainTable->setColumnWidth(0,40);
    ui->mainTable->setColumnWidth(1,230);
    ui->mainTable->setColumnWidth(2,230);
    ui->mainTable->setColumnWidth(3,230);
    ui->mainTable->setColumnWidth(4,230);

    QString path = ":/Questions/Questions.dat";     //应该好了吧
    QFile f(path);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "提示",
                             QString("源文件读取失败，请检查目录中是否包含Questions.dat\n\n路径：%1\n错误：%2")
                                 .arg(path).arg(f.errorString()));
        return;
    }
    QTextStream txt(&f);
    for(int i = 0;i < 21;i++)
    {
        Question* q = new Question();
        txt >> q->descriptions[0] >> q->descriptions[1] >> q->descriptions[2] >> q->descriptions[3];

        questions.push_back(q);
        qDebug() << "Write question" << i + 1 << "successfully,description = " << q->descriptions;
    }
    f.close();

    pull(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::beck()
{

}

void MainWindow::pull(int index)//index为页码号;向表格推送题目
{
    int num = 7 * (index - 1);
    if(index <= 0)
    {
        qDebug() << "False index" ;
        return;
    }

    ui->mainTable->setRowCount(0);
    //debug();
    for(int i = 0; i < 7;i++)
    {
        ui->mainTable->insertRow(i);//要先插入行才能继续插入内容
        ui->mainTable->setRowHeight(i,60);
        ui->mainTable->setItem(i,0,new QTableWidgetItem(QString::number(num + i + 1)));
        qDebug() << questions.at(i + num)->descriptions;
        QButtonGroup* buttonGroup = new QButtonGroup();//一个按钮组，放在这玩，过两天拿出来炒菜吃。一定要放一点青椒，炒出来更香。别问为什么，问就是好这口。好吃！
        QRadioButton* non = new QRadioButton(questions.at(i + num)->descriptions[0]);
        QRadioButton* sel = new QRadioButton(questions.at(i + num)->descriptions[1]);
        QRadioButton* mod = new QRadioButton(questions.at(i + num)->descriptions[2]);
        QRadioButton* freq = new QRadioButton(questions.at(i + num)->descriptions[3]);
        buttonGroup->addButton(non,0);
        buttonGroup->addButton(sel,1);
        buttonGroup->addButton(mod,2);
        buttonGroup->addButton(freq,3);
        ui->mainTable->setCellWidget(i,1,non);
        ui->mainTable->setCellWidget(i,2,sel);
        ui->mainTable->setCellWidget(i,3,mod);
        ui->mainTable->setCellWidget(i,4,freq);

        qDebug() << "Call question" << num + i + 1 << "successfully,description = " << questions.at(i + num)->descriptions << questions.at(i + num)->score;

        if(questions[i + num]->score != -1)//作答就一定有得分罢（无端
        {
            buttonGroup->button(questions[i + num]->score - 1)->setChecked(1);
        }//一个按钮，一个被创造的按钮，它的勾选状态是持续的。这将说明什么？没有人知道。
    }
}

void MainWindow::push(int index)//将题目作答情况推送至questions数组
{
    int num = 7 * (index - 1);
    for(int i = 0;i < 7;i++)
    {
        for(int j = 0;j < 4;j++)//又是嵌套。
        {
            QRadioButton* bt = static_cast<QRadioButton*>(ui->mainTable->cellWidget(i,j+1));
            if(bt->isChecked() == 1)
            {
                questions.at(i + num)->score = j + 1;
            }
        }
    }
}

void MainWindow::on_prePage_clicked()
{
    ui->submit->hide();
    ui->nextPage->show();
    if(currentPage == 1)
    {
        ui->hint->setText("已经是第一页");
        return;
    }
    else
    {
        ui->hint->setText("      ");//6个空格
        push(currentPage);
        currentPage--;
        pull(currentPage);
        ui->pageHint->setText((QString("当前页码：%1 / 3")).arg(currentPage));
    }
}

void MainWindow::on_nextPage_clicked()
{
    int num = 7 * (currentPage - 1);
    bool isFinished = 1;
    push(currentPage);
    QString notFin;
    if(currentPage == 3)
    {
        ui->submit->show();
        ui->nextPage->hide();
        ui->hint->setText("已经是最后一页。准备提交结果吗？没事的，放轻松。");
        return;
    }
    else
    {
        for(int i = 0;i < 7;i++)
        {
            bool flag = 0;
            if(questions[i + num]->score != -1)
            {
                flag = 1;
            }
            if(flag == 0)
            {
                notFin.push_back(QString::number(num + i + 1) + ' ');
                if(isFinished == 1)
                {
                    isFinished = 0;
                }
            }
        }
        if(!isFinished)
        {
            QMessageBox::warning(this,"提示",("本页您还有题目未完成，题号为：\n" + notFin + "\n请先完成本页全部题目"));
            ui->hint->setText("有题目未完成");
            return;
        }
        ui->hint->setText("      ");//6个空格
        push(currentPage);
        currentPage++;
        pull(currentPage);
        ui->pageHint->setText((QString("当前页码：%1 / 3")).arg(currentPage));
    }
}

void MainWindow::on_submit_clicked()
{
    bool isFinished = 1;
    push(currentPage);
    QString notFin;
    for(int i = 0;i < 7;i++)
    {
        bool flag = 0;
        if(questions[14 + i]->score != -1)
        {
            flag = 1;
        }
        if(flag == 0)
        {
            notFin.push_back(QString::number(14 + i + 1) + ' ');
            if(isFinished == 1)
            {
                isFinished = 0;
            }
        }
    }
    if(!isFinished)
    {
        QMessageBox::warning(this,"提示",("本页您还有题目未完成，题号为：\n" + notFin + "\n请先完成本页全部题目"));
        ui->hint->setText("有题目未完成");
        return;
    }
    QMessageBox::information(this,"提示","提交成功！点击“OK”查看结果。");
    calculate();
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::calculate()
{
    for(auto const& q : questions)
    {
        score += q->score;
    }
    ui->sum->setText(QString::number(score));
}

void MainWindow::on_Agreement_clicked()
{
    Agreement* agr = new Agreement(this);
    agr->setWindowTitle("用户使用协议");
    if(agr->exec() == QDialog::Accepted)
    {
        ui->checkBox->setEnabled(1);
    }
    delete agr;
}

void MainWindow::on_ins_clicked()
{
    Instruction* i = new Instruction(this);
    i->setWindowTitle("量表简介");
    if(i->exec() == QDialog::Accepted)
    {

    }
    delete i;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->Enter->setEnabled(arg1);
}

void MainWindow::on_Enter_clicked()
{
    this->setWindowTitle("贝克抑郁量表");
    ui->stackedWidget->setCurrentIndex(1);
}

