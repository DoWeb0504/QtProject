#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "boss.h"
#include "apply.h"
#include "viewer.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    void showBossPwd();
    void dealToLogin(QString);

signals:
   // sendNameAndPhone(QString name,QString phone);

private slots:
    void dealBossToLogin();

    void on_btnUser_clicked();

    void on_btnToRegist_clicked();

    void on_btnBacktoMain_clicked();

    void on_btnBacktoLogin_clicked();

    void on_btnLogin_clicked();

    void on_btnRegist_clicked();

    void on_btnCancel_clicked();

    void on_btnInter_clicked();

    void on_btnInterToRes_clicked();

    void on_btnInterToMain_clicked();

    void on_btnInterToLogin_clicked();

    void on_btnInterResCancel_clicked();

    void on_btnInterCancel_clicked();

    void on_btnInterLogin_clicked();

    void on_btnInterRes_clicked();

    void on_btnBoss_clicked();

    void on_btnBossBackToMain_clicked();

    void on_btnBossClear_clicked();

    void on_btnBossEnter_clicked();

    void on_pushButton_clicked();

    void on_btnBossChangeToEnter_clicked();

    void on_btnBosChangePwdCancel_clicked();

    void on_btnBossChangePwd_clicked();

private:
    Ui::Login *ui;

    int bossEnterTimes;

    Boss bossWin;  //boss子窗口
    apply applyWin;  //应聘生子窗口
    viewer viewerWin;  //面试官子窗口
};

#endif // LOGIN_H
