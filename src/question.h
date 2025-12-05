#ifndef QUESTION_H
#define QUESTION_H

#include <QObject>
#include <QWidget>

class Question
{


public:

    Question();
    ~Question();


    QVector<QString> descriptions;//本题描述内容
    int score;//本题得分
};

#endif // QUESTION_H
