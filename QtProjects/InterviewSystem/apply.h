#ifndef APPLY_H
#define APPLY_H

#include <QMainWindow>
//#include "login.h"

namespace Ui {
class apply;
}

class apply : public QMainWindow
{
    Q_OBJECT

public:
    explicit apply(QWidget *parent = 0);
    ~apply();

    void initData(); //初始化数据

    void initHome();//个人主页初始化

public:
    QString myName;
    QString myPhone;

signals:
    void changeToLoginA(QString);

private slots:

    void on_btnJoinUs_clicked();

    void on_btnLearnMore_clicked();

    void on_btnBackToMain_clicked();

    void on_comboBoxCategory_currentIndexChanged(const QString &arg1);

    void on_comboBoxJobs_currentIndexChanged(const QString &arg1);

    void on_btnApply_2_clicked();

    void on_btnApply_clicked();

    void on_pushButton_clicked();

    void on_btnModify_clicked();

    void on_btnSign_clicked();

    void on_btnExit_clicked();

private:
    Ui::apply *ui;
    QString signStatus;
    QString notSignStatus;
    QString unFinishStatus;
    QString passStatus;
    QString notPassStatus;

};

#endif // APPLY_H
