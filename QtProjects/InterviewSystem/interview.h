#ifndef INTERVIEW_H
#define INTERVIEW_H

#include <QWidget>
#include <QTimer>
#include <QTime>

namespace Ui {
class InterView;
}

class InterView : public QWidget
{
    Q_OBJECT

public:
    explicit InterView(QWidget *parent = 0);
    ~InterView();

    void dealShow();
    void updateTime();

public:
    QString myName;
    QString myPhone;
    QString myProfess;
    QString mySchool;
    QString myYear;
    QString myJob;

signals:
    void timeoutT(QString);

private slots:
    void on_btnStartTime_clicked();

    void on_btnEndTime_clicked();

private:
    Ui::InterView *ui;
    QTimer *qtimer;   //声明一个定时器对象
    QTime baseTime;  //声明一个时间对象
    QString showStr;  //直接声明
};

#endif // INTERVIEW_H
