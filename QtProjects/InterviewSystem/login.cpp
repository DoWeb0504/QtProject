#include "login.h"
#include "ui_login.h"
#include "pubfuns.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegExp>
#include <QTcpSocket>
#include <QPixmap>



Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->pageMain);  //一开始显示主界面
    setFixedSize(400,400);

    //设置背景
    QPixmap pixmap = QPixmap(":/image/image/login.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);

    QString button_style = "QPushButton{background-color:rgb(85,170,255);\
                            color:white;border-radius:10px;border:2px groove gray;\
                            border-style:outset;}"
                            "QPushButton:hover{background-color:white;color:black}"
                            "QPushButton:pressd{background-color:blue;\
                                border-style:inset;}";  //按键样式表

    ui->btnBoss->setStyleSheet(button_style);
    ui->btnInter->setStyleSheet(button_style);
    ui->btnUser->setStyleSheet(button_style);
    ui->btnLogin->setStyleSheet(button_style);
    ui->btnRegist->setStyleSheet(button_style);
    ui->btnCancel->setStyleSheet(button_style);
    ui->btnBacktoLogin->setStyleSheet(button_style);
    ui->btnBacktoMain->setStyleSheet(button_style);
    ui->btnBosChangePwdCancel->setStyleSheet(button_style);
    ui->btnBossBackToMain->setStyleSheet(button_style);
    ui->btnBossChangePwd->setStyleSheet(button_style);
    ui->btnBossChangeToEnter->setStyleSheet(button_style);
    ui->btnInter->setStyleSheet(button_style);
    ui->btnInterCancel->setStyleSheet(button_style);
    ui->btnInterLogin->setStyleSheet(button_style);
    ui->btnInterToMain->setStyleSheet(button_style);
    ui->btnInterToRes->setStyleSheet(button_style);
    ui->btnInterResCancel->setStyleSheet(button_style);
    ui->btnInterToLogin->setStyleSheet(button_style);
    ui->btnInterToMain->setStyleSheet(button_style);
    ui->btnInterToRes->setStyleSheet(button_style);
    ui->btnToRegist->setStyleSheet(button_style);
    ui->btnBossEnter->setStyleSheet(button_style);
    ui->btnBossClear->setStyleSheet(button_style);
    ui->pushButton->setStyleSheet(button_style);


    bossEnterTimes = 0;
    QRegExp regExp("^[0-9]{11}$");
    QRegExpValidator *pattern = new QRegExpValidator(regExp,this);
    ui->lineEditUserPhone->setValidator(pattern);  //限定只能输入11位数字
    ui->lineEditResPhone->setValidator(pattern);

    QRegExp regExpName("[\u4e00-\u9fa5]{3,4}");  //只能输入中文
    QRegExpValidator *patternName = new QRegExpValidator(regExpName,this);
    ui->lineEditResName->setValidator(patternName);
    ui->lineEditInterResName->setValidator(patternName);

    QRegExp regExpInter("^[0-9]{10}$");
    QRegExpValidator *patternInter = new QRegExpValidator(regExpInter,this);
    ui->lineEditInterNum->setValidator(patternInter);
    ui->lineEditInterResNum->setValidator(patternInter);




    void (Boss::*changeToLoginB)(QString) = &Boss::changeToLoginB;
    connect(&bossWin,changeToLoginB,this,&Login::dealToLogin);

    void (viewer::*changeToLoginV)(QString) = &viewer::changeToLoginV;
    connect(&viewerWin,changeToLoginV,this,&Login::dealToLogin);

    void (apply::*changeToLoginA)(QString) = &apply::changeToLoginA;
    connect(&applyWin,changeToLoginA,this,&Login::dealToLogin);
//    void (Login::*funSignal)(QString,QString) = &Login::sendNameAndPhone;
//    connect(this,funSignal,applyWin,&apply::dealSignalNameAndPhone);
}

void Login::dealToLogin(QString arg1){
    if(arg1 == "boss"){
        bossWin.close();
    }else if(arg1 == "viewer"){
        viewerWin.close();
    }else if(arg1 == "apply"){
        applyWin.close();
    }else return;
    this->show();
    ui->stackedWidget->setCurrentWidget(ui->pageMain);  //一开始显示主界面
}

//信号槽函数
void Login::dealBossToLogin(){
    bossWin.close();
    this->show();
    ui->stackedWidget->setCurrentWidget(ui->pageMain);  //一开始显示主界面
}

Login::~Login()
{
    delete ui;
}

/*******************
 * 应聘生操作
 * ****************/
//应聘生按键
void Login::on_btnUser_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageLogin);
    ui->lineEditUserPhone->clear();
    ui->lineEditUserPassword->clear();
}
//登陆界面转到注册界面
void Login::on_btnToRegist_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageRegist);
    on_btnCancel_clicked();//清除注册框
}
//登陆界面返回主界面
void Login::on_btnBacktoMain_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}
//注册页面返回登陆界面
void Login::on_btnBacktoLogin_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageLogin);
    ui->lineEditUserPhone->clear();
    ui->lineEditUserPassword->clear();
}
//登陆按键
void Login::on_btnLogin_clicked()
{
    QString userPhone = ui->lineEditUserPhone->text();
    QString userPwd = ui->lineEditUserPassword->text();
    if(userPhone.length() == 0 || userPwd.length() == 0){
        QMessageBox::warning(this,"信息不完整","请完善信息",QMessageBox::Ok);
        return;
    }
    if(userPhone.length() != 11){
        QMessageBox::warning(this,"号码错误","请输入正确的号码",QMessageBox::Ok);
        return;
    }
    QSqlQuery query;
    QString sql = QString("select * from login");
    //linkDatabase(query,sql);
    query.exec(sql);
    while(query.next()){
        if(query.value("phone") == userPhone){
            if(query.value("password") == userPwd){
                //登陆成功动作
                QMessageBox::information(this,"欢迎","欢迎来到DaeQt开发有限公司");
                applyWin.setFixedSize(900,580);
                applyWin.show();
                applyWin.myName = query.value("name").toString();
                applyWin.myPhone = userPhone;
                //emit sendNameAndPhone(query.value("name").toString(),userPhone);
                this->close();
                return;
            }else{
                //密码错误弹窗
                ui->lineEditUserPassword->clear();
                QMessageBox::warning(this,"warning","密码错误");
                return;
            }
        }
    }
    //未注册弹框
    if(QMessageBox::Yes == QMessageBox::question(this,"Question","你的号码未进行注册，是否进行注册",QMessageBox::Yes|QMessageBox::No)){
        ui->stackedWidget->setCurrentWidget(ui->pageRegist);
        on_btnCancel_clicked();//清除注册框
    }else{
        ui->lineEditUserPhone->clear();
        ui->lineEditUserPassword->clear();
    }
}
//注册键
void Login::on_btnRegist_clicked()
{

    QString resName = ui->lineEditResName->text();
    QString resPhone = ui->lineEditResPhone->text();
    QString resPassword = ui->lineEditResPwd->text();
    QString resSurePassword = ui->lineEditResSurePwd->text();
    if(resPhone.length() == 0 || resName.length() == 0 || resPassword.length() == 0 || resSurePassword.length() == 0) {  //信息不完整
        QMessageBox::warning(this,"信息不完整","请完整个人信息！",QMessageBox::Yes);
        return;
    }
    if(!(resPhone.length() == 11)){ //判断号码的位数
        QMessageBox::warning(this,"号码错误","请输入正确的电话号码！",QMessageBox::Yes);
        ui->lineEditResPhone->clear();
        return;
    }
    if(resName == NULL){  //判断姓名为空
        QMessageBox::warning(this,"姓名为空","请输入你的姓名！",QMessageBox::Yes);
        ui->lineEditResName->clear();
        return;
    }
    if(resPassword.length() < 6){//密码严密性检测
        QMessageBox::warning(this,"密码简单","请保证密码不少于6个字符",QMessageBox::Yes);
        ui->lineEditResPwd->clear();
        ui->lineEditResSurePwd->clear();
        return;
    }
    if(resPassword != resSurePassword){  //检验密码一致性
        QMessageBox::warning(this,"warning","两次密码输入不一致");
        ui->lineEditResPwd->clear();
        ui->lineEditResSurePwd->clear();
        return;
    }
    QSqlQuery query;
    query.exec("select * from login");
    while(query.next()){
        if(query.value("phone")==resPhone){
            QMessageBox::warning(this,"error","该号码已经被注册");
            on_btnCancel_clicked();
            return;
        }
        if(query.value("name")==resName){
            QMessageBox::warning(this,"error","您已经注册，请直接登陆");
            ui->stackedWidget->setCurrentWidget(ui->pageLogin);
            ui->lineEditUserPhone->clear();
            ui->lineEditUserPassword->clear();
        }
    }

    QString sql = QString("insert into login(name,phone,password) values('%1','%2','%3')")
            .arg(resName)
            .arg(resPhone)
            .arg(resPassword);
    //开启一个事务
    QSqlDatabase::database().transaction();
    query.exec(sql);
    if(QMessageBox::Yes == QMessageBox::information(this,"注册","确定注册",QMessageBox::Yes|QMessageBox::No)){
        QSqlDatabase::database().commit();  //确认注册
        on_btnCancel_clicked();
        if(QMessageBox::Yes == QMessageBox::information(this,"成功","注册成功现在进行登陆?",QMessageBox::Yes|QMessageBox::No)){
            ui->stackedWidget->setCurrentWidget(ui->pageLogin);
            ui->lineEditUserPassword->clear();
            ui->lineEditUserPhone->setText(resPhone);
        }
    }else{
        QSqlDatabase::database().rollback();  //取消注册
    }
}
//注册取消键
void Login::on_btnCancel_clicked()
{
    ui->lineEditResName->clear();
    ui->lineEditResPhone->clear();
    ui->lineEditResPwd->clear();
    ui->lineEditResSurePwd->clear();
}


/***************
 * 面试官操作
 * **************/
//面试官按键
void Login::on_btnInter_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageInterLogin);
    ui->lineEditInterNum->clear();
    ui->lineEditInterPwd->clear();
}
//登陆转注册
void Login::on_btnInterToRes_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageInteRes);
    on_btnInterResCancel_clicked();

}
//面试官登陆转主界面
void Login::on_btnInterToMain_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}
//面试官注册转登陆
void Login::on_btnInterToLogin_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageInterLogin);
    ui->lineEditInterNum->clear();
    ui->lineEditInterPwd->clear();
}
//登陆取消键
void Login::on_btnInterCancel_clicked()
{
    ui->lineEditInterNum->clear();
    ui->lineEditInterPwd->clear();
}
//登陆按键
void Login::on_btnInterLogin_clicked()
{
    QString interNum = ui->lineEditInterNum->text();
    QString interPwd = ui->lineEditInterPwd->text();
    if(interPwd.length() == 0 || interNum.length() == 0){
        QMessageBox::warning(this,"信息未完整","请完整信息",QMessageBox::Ok);
        return;
    }
    QSqlQuery query;
    query.exec("select * from Inter");
    while(query.next()){
        if(query.value("num") == interNum){
            QString sqlInterPwd = query.value("password").toString();
            if(sqlInterPwd.length() == 0){ //判断有无注册
                if(QMessageBox::Yes == QMessageBox::warning(this,"未注册","你还未注册，是否进行注册",QMessageBox::Yes|QMessageBox::No)){
                    on_btnInterToRes_clicked(); //转至注册
                    ui->lineEditInterResNum->setText(interNum);
                }else{
                    on_btnInterCancel_clicked();
                }
                return;
            }
            if(query.value("password") == interPwd){
                QMessageBox::information(this,"登陆成功","登陆成功",QMessageBox::Yes);
                viewerWin.show();
                this->close();
//                QMessageBox::about(this,"更新","该分支功能正在全速开发中....，敬请期待哈！");
                on_btnInterCancel_clicked();
                return;
            }else{
                QMessageBox::warning(this,"密码错误","请输入正确的密码",QMessageBox::Ok);
                ui->lineEditInterPwd->clear();
                return;
            }
        }
    }
    QMessageBox::critical(this,"坏人","你不是本公司面试官，请速离开",QMessageBox::Ok);
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}
//注册按键
void Login::on_btnInterRes_clicked()
{
    QString interResNum = ui->lineEditInterResNum->text();
    QString interResName = ui->lineEditInterResName->text();
    QString interResPwd = ui->lineEditInterResPwd->text();
    QString interResSurePwd = ui->lineEditInterResSurePwd->text();
    if(interResName.length() == 0 || interResPwd.length() == 0
            || interResSurePwd.length() == 0 || interResNum.length() != 10){
        QMessageBox::warning(this,"信息不完整","请完善信息！",QMessageBox::Ok);
        return;
    }
    if(interResPwd.length() < 6){
        QMessageBox::warning(this,"不安全","请保证密码不少于6个字符",QMessageBox::Ok);
        ui->lineEditInterResPwd->clear();
        ui->lineEditInterResSurePwd->clear();
        return;
    }
    if(interResPwd != interResSurePwd){ //密码不一致
        QMessageBox::warning(this,"错误","请确保两次输入密码一致",QMessageBox::Ok);
        ui->lineEditInterResPwd->clear();
        ui->lineEditInterResSurePwd->clear();
        return;
    }
    QSqlQuery query;
    query.exec("select num from Inter");
    while(query.next()){
        if(query.value("num") == interResNum){
            QString sql = QString("update Inter set name = '%1', password = '%2' where num = '%3'")
                    .arg(interResName)
                    .arg(interResPwd)
                    .arg(interResNum);
            //开启一个事务
            QSqlDatabase::database().transaction();
            query.exec(sql);
            if(QMessageBox::Yes == QMessageBox::information(this,"注册","确定注册",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)){
                QSqlDatabase::database().commit();  //确认注册
                if(QMessageBox::Yes == QMessageBox::information(this,"成功","注册成功，现在进行登陆？",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)){
                    on_btnInterToLogin_clicked();  //进行登陆
                    ui->lineEditInterNum->setText(interResNum);
                }
                return;
            }else{
                QSqlDatabase::database().rollback();  //取消
                return;
            }
        }
    }
    QMessageBox::critical(this,"坏人","你是假的面试官，走吧",QMessageBox::Ok);
}
//注册取消键
void Login::on_btnInterResCancel_clicked()
{
    ui->lineEditInterResName->clear();
    ui->lineEditInterResNum->clear();
    ui->lineEditInterResPwd->clear();
    ui->lineEditInterResSurePwd->clear();
}

/**************************
 * Boss操作
 * **********************/
//BigBoss按键
void Login::on_btnBoss_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageBossLogin);
    ui->lineEditBoss->clear();
    showBossPwd();
}
//登陆转主界面
void Login::on_btnBossBackToMain_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}
//清除按键
void Login::on_btnBossClear_clicked()
{
    ui->lineEditBoss->clear();
}
//进入按键
void Login::on_btnBossEnter_clicked()
{
    if(bossEnterTimes > 2){
        QMessageBox::critical(this,"警告","你怎么还不走呀，别调皮啦哈",QMessageBox::Ok);
        on_btnBossBackToMain_clicked();
        return;
    }
    QString bossPwd = ui->lineEditBoss->text();
    if(bossPwd.length() == 0){
        QMessageBox::warning(this,"没有口令","请输入口令",QMessageBox::Ok);
        return;
    }
    QSqlQuery query;
    QString sql = QString("select password from boss where id = 1 ;");
    query.exec(sql);
    while(query.next()){
        if(query.value("password") == bossPwd){
            QMessageBox::information(this,"进入","欢迎BigBoss",QMessageBox::Ok);
            bossEnterTimes = 0;
            bossWin.show();
            this->close(); //切换到boss管理窗口
            return;
        }
    }
    bossEnterTimes ++;
    if(bossEnterTimes == 3){
        QMessageBox::critical(this,"警告","你根本不是Boss,走吧",QMessageBox::Ok);
        on_btnBossBackToMain_clicked();
        return;
    }
    QMessageBox::warning(this,"口令错误",tr("请确定口令正确\n你还有 %1 次机会").arg(3 - bossEnterTimes),QMessageBox::Ok);
    ui->lineEditBoss->clear();

}
//登陆->更改口令按键
void Login::on_pushButton_clicked()
{
    if(bossEnterTimes > 2){
        QMessageBox::critical(this,"警告","你怎么还不走呀，别调皮啦哈",QMessageBox::Ok);
        on_btnBossBackToMain_clicked();
        return;
    }
    QString bossPwd = ui->lineEditBoss->text();
    if(bossPwd.length() == 0){
        QMessageBox::warning(this,"没有口令","请输入旧口令",QMessageBox::Ok);
        return;
    }
    QSqlQuery query;
    QString sql = QString("select password from boss where id = 1 ;");
    query.exec(sql);
    while(query.next()){
        if(query.value("password") == bossPwd){
            ui->stackedWidget->setCurrentWidget(ui->pageBossChangePwd);
            on_btnBosChangePwdCancel_clicked();
            bossEnterTimes = 0;
            return;
        }
    }
    bossEnterTimes ++;
    if(bossEnterTimes == 3){
        QMessageBox::critical(this,"警告","你根本不是Boss,走吧",QMessageBox::Ok);
        on_btnBossBackToMain_clicked();
        return;
    }
    QMessageBox::warning(this,"口令错误",tr("请确定口令正确\n你还有 %1 次机会").arg(3 - bossEnterTimes),QMessageBox::Ok);
    ui->lineEditBoss->clear();
}
//更改口令返回进入界面
void Login::on_btnBossChangeToEnter_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageBossLogin);
    ui->lineEditBoss->clear();
    showBossPwd();
}
//更换口令取消键
void Login::on_btnBosChangePwdCancel_clicked()
{
    ui->lineEditBossNewPwd->clear();
    ui->lineEditBossNewSurePwd->clear();
}
//更换口令确定键
void Login::on_btnBossChangePwd_clicked()
{
    bossEnterTimes = 0;
    QString bossNewPwd = ui->lineEditBossNewPwd->text();
    QString bossNewSurePwd = ui->lineEditBossNewSurePwd->text();
    if(bossNewPwd.length() == 0 || bossNewSurePwd == 0){
        QMessageBox::warning(this,"信息不完整","请完整信息!",QMessageBox::Ok);
        return;
    }
    if(bossNewPwd.length() < 6){
        QMessageBox::warning(this,"错误","请确保口令不少于6个字符！",QMessageBox::Ok);
        return;
    }
    if(bossNewPwd == bossNewSurePwd){
        QSqlQuery query;
        QString sql = QString("select password from boss where id = 1 ;");
        query.exec(sql);
        while(query.next()){
            if(query.value("password") == bossNewPwd){
                QMessageBox::warning(this,"旧口令","口令未更改",QMessageBox::Ok);
                return;
            }
        }
        //开启一个事务
        QSqlDatabase::database().transaction();
        sql = QString("update boss set password = '%1' where id = 1").arg(bossNewPwd);
        query.exec(sql);
        if(QMessageBox::Yes == QMessageBox::information(this,"确认","确定修改口令？",QMessageBox::Yes|QMessageBox::No)){
            QSqlDatabase::database().commit();
            QMessageBox::information(this,"成功","口令修改成功",QMessageBox::Ok);
            on_btnBossChangeToEnter_clicked();
            return;
        }else{
            QMessageBox::warning(this,"取消","口令未修改",QMessageBox::Yes);
            return;
        }
    }else{
        QMessageBox::warning(this,"错误","口令确认不一致，请重新确认",QMessageBox::Ok);
        ui->lineEditBossNewSurePwd->clear();
    }
}
//显示boss口令
void Login::showBossPwd()
{
    QSqlQuery query;
    QString sql = QString("select password from boss where id = 1");
    query.exec(sql);
    while(query.next()){
        ui->labelBoss->setText(query.value("password").toString());
    }

}
