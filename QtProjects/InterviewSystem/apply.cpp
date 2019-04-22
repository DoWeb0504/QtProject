#include "apply.h"
#include "ui_apply.h"
#include "pubfuns.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QPixmap>



apply::apply(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::apply)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
    setWindowTitle("DaeQt");
    //设置背景
    QPixmap pixmap = QPixmap(":/image/image/apply.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);

    QString button_style = "QPushButton{background-color:rgb(72,209,204);\
                            color:white;border-radius:10px;border:2px groove gray;\
                            border-style:outset;}"
                            "QPushButton:hover{background-color:white;color:black}"
                            "QPushButton:pressd{background-color:green;\
                             border-style:inset;}";  //按键样式表

    QString button_style1 = "QPushButton{background-color:rgb(124,252,0);\
                            color:white;border-radius:10px;border:2px groove gray;\
                            border-style:outset;}"
                            "QPushButton:hover{background-color:white;color:black}"
                            "QPushButton:pressd{background-color:green;\
                             border-style:inset;}";  //按键样式表
    ui->btnApply->setStyleSheet(button_style);
    ui->btnApply_2->setStyleSheet(button_style);
    ui->btnBackToMain->setStyleSheet(button_style);
    ui->btnExit->setStyleSheet(button_style);
    ui->btnJoinUs->setStyleSheet(button_style);
    ui->btnLearnMore->setStyleSheet(button_style);
    ui->btnModify->setStyleSheet(button_style);
    ui->btnSign->setStyleSheet(button_style1);
    ui->pushButton->setStyleSheet(button_style);

    QString comBox_style = "QComboBox{border:1px solid #d7d7d7; border-radius: 3px; padding: 1px 18px 1px 3px;}"
                           "QComboBox:editable{ background: white; }"
                           "QComboBox:!editable{ background: #7AC5CD; color:#EE0000}"
                           "QComboBox::drop-down{ subcontrol-origin: padding; subcontrol-position: top right; width: 22px; border-left-width: 1px;\
                           border-left-color: #c9c9c9; border-left-style: solid; /* just a single line */ border-top-right-radius: 3px; /* same radius as the QComboBox */border-bottom-right-radius: 3px; }"
                           "QComboBox::down-arrow { image: url(:/down.png); }"
                           "QComboBox::down-arrow:on { /* shift the arrow when popup is open */ top: 1px; left: 1px;}"
                           "QComboBox QAbstractItemView::item{max-width: 30px;min-height: 20px}";

    ui->comboBoxCategory->setStyleSheet(comBox_style);
    ui->comboBoxJobs->setStyleSheet(comBox_style);
    ui->comboBoxYear->setStyleSheet(comBox_style);

    //openDatabase();
    initData();
    signStatus = "已签到";
    notSignStatus = "未签到";
    unFinishStatus = "未完成";
    passStatus = "已通过";
    notPassStatus = "未通过";

}

apply::~apply()
{
    delete ui;
}

//初始化数据
void apply::initData()
{
    QSqlQueryModel *queryModel = new QSqlQueryModel(this); //新建模型，放置数据库数据
    QSqlQueryModel *queryModelYaer = new QSqlQueryModel(this); //新建模型，放置数据库数据
    queryModel->setQuery("select name from category");
    ui->comboBoxCategory->setModel(queryModel);  //放进岗位类别下拉框
    queryModelYaer->setQuery("select year from category");
    ui->comboBoxYear->setModel(queryModelYaer);
    ui->lineEditName->setEnabled(false);
    ui->lineEditPhone->setEnabled(false);

}
//加入我们
void apply::on_btnJoinUs_clicked()
{
    on_btnApply_2_clicked();
    ui->comboBoxYear->setCurrentText("2020");
    ui->lineEditName->setText(myName);
    ui->lineEditPhone->setText(myPhone);
    QSqlQuery query;
    QString sql = QString("select name from applicant;");
    query.exec(sql);
    while(query.next()){
        if(query.value("name") == myName){  //已提交申请
            ui->stackedWidget->setCurrentWidget(ui->pageUserHome);//直接跳转至个人主页
            initHome();
            return;
        }
    }
    ui->stackedWidget->setCurrentWidget(ui->pageJobs);

}

void apply::initHome()
{
    ui->lineEditName_H->setEnabled(false);
    ui->lineEditPhone_H->setEnabled(false);
    ui->lineEditProfess_H->setEnabled(false);
    ui->lineEditSchool_H->setEnabled(false);
    ui->lineEditYear_H->setEnabled(false);
    ui->lineEditCategory_H->setEnabled(false);
    ui->lineEditJob_H->setEnabled(false);
    QSqlQuery query;
    QString sql = QString("select * from applicant where name = '%1';").arg(myName);
    query.exec(sql);
    while(query.next()){
        //初始化应聘进度
        ui->lineEditName_H->setText(myName);
        ui->lineEditPhone_H->setText(myPhone);
        ui->lineEditProfess_H->setText(query.value("profess").toString());
        ui->lineEditSchool_H->setText(query.value("school").toString());
        ui->lineEditYear_H->setText(query.value("year").toString());
        ui->lineEditCategory_H->setText(query.value("category").toString());
        ui->lineEditJob_H->setText(query.value("job").toString());
        if(query.value("sign").toInt() == 0){
            ui->labelStatusSign->setText(notSignStatus);
            ui->labelPicSign->clear();
            ui->btnSign->setEnabled(true);
        }else{
            ui->labelStatusSign->setText(signStatus);
            ui->labelPicSign->setPixmap(QPixmap(":/image/image/sign.png").scaled(ui->labelPicSign->size()));
            ui->btnSign->setEnabled(false);
        }
        if(query.value("pass1").toInt() == 0){
           ui->labelStatusPass1->setText(unFinishStatus);
        }else if(query.value("pass1").toInt() == 1){
            ui->labelStatusPass1->setText(passStatus);
            ui->labelPicPass1->setPixmap(QPixmap(":/image/image/1.png").scaled(ui->labelPicPass1->size()));
        }else{
            ui->labelStatusPass1->setText(notPassStatus);
            ui->labelPicPass1->setPixmap(QPixmap(":/image/image/out.png").scaled(ui->labelPicPass1->size()));
            ui->btnSign->setEnabled(false);
        }
        if(query.value("pass2").toInt() == 0){
            ui->labelStatusPass2->setText(unFinishStatus);
        }else if(query.value("pass2").toInt() == 1){
            ui->labelStatusPass2->setText(passStatus);
            ui->labelPicPass2->setPixmap(QPixmap(":/image/image/2.png").scaled(ui->labelPicPass2->size()));
        }else{
            ui->labelStatusPass2->setText(notPassStatus);
            ui->labelPicPass2->setPixmap(QPixmap(":/image/image/out.png").scaled(ui->labelPicPass2->size()));
            ui->btnSign->setEnabled(false);
        }
        if(query.value("passHR").toInt() == 0){
            ui->labelStatusPassHR->setText(unFinishStatus);
        }else if(query.value("passHR").toInt() == 1){
            ui->labelStatusPassHR->setText(passStatus);
            ui->labelPicPassHR->setPixmap(QPixmap(":/image/image/HR.png").scaled(ui->labelPicPassHR->size()));
        }else{
            ui->labelStatusPassHR->setText(notPassStatus);
            ui->labelPicPassHR->setPixmap(QPixmap(":/image/image/out.png").scaled(ui->labelPicPassHR->size()));
            ui->btnSign->setEnabled(false);
        }
    }
}

//了解更多
void apply::on_btnLearnMore_clicked()
{
    QMessageBox::about(this,"更新","该功能正在开发中，敬请期待！");
}
//返回主界面
void apply::on_btnBackToMain_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}
//类别下拉框选择槽函数
void apply::on_comboBoxCategory_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "请选择岗位类别"){
        ui->comboBoxJobs->clear(); //岗位下拉框清空
        ui->labelPic->clear();
        ui->labelPic->setText("职位简介");
    }else{
        ui->comboBoxJobs->clear();
        QSqlQuery query;
        QString sql = QString("select job from jobs where category = '%1'").arg(arg1);
        query.exec(sql);
        while(query.next()){
            QString name = query.value(0).toString();
            ui->comboBoxJobs->addItem(name);  //岗位下拉框显示内容
        }
    }
}
//岗位下拉框选择槽函数
void apply::on_comboBoxJobs_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query;
    QString sql = QString("select intro from jobs where category = '%1' and job = '%2'")
            .arg(ui->comboBoxCategory->currentText())
            .arg(arg1);
    query.exec(sql);
    while(query.next()){
        QPixmap photo;
        photo.loadFromData(query.value("intro").toByteArray(),"PNG");
        ui->labelPic->setPixmap(QPixmap(photo).scaled(ui->labelPic->size()));
    }
}
//取消按键
void apply::on_btnApply_2_clicked()
{
    ui->lineEditProfess->clear();
    ui->lineEditSchool->clear();
    ui->comboBoxCategory->setCurrentText("请选择岗位类别");
    ui->comboBoxYear->setCurrentText("2020");
}
//申请
void apply::on_btnApply_clicked()
{
    bool Ok = false;
    QSqlQuery query;
    QString sql = QString("select modifyEN from boss where id = 1");
    query.exec(sql);
    while(query.next()){  //判断是否允许提交申请
        if(0 == query.value("modifyEN").toInt()){
            QMessageBox::warning(this,"通知","申请已截止提交",QMessageBox::Yes);
            return;
        }
    }
    QString name = ui->lineEditName->text();
    QString phone = ui->lineEditPhone->text();
    QString profess = ui->lineEditProfess->text();
    QString school = ui->lineEditSchool->text();
    QString year = ui->comboBoxYear->currentText();
    QString cate = ui->comboBoxCategory->currentText();
    QString job = ui->comboBoxJobs->currentText();
    if(profess.length() == 0 || school.length() == 0 || job.length() == 0){
        QMessageBox::warning(this,"信息不完整","请确保信息完整",QMessageBox::Ok);
        return;
    }
    if(QMessageBox::No == QMessageBox::information(this,"确认","确认提交？",QMessageBox::Yes|QMessageBox::No)) return;
    sql = QString("select phone from applicant");
    query.exec(sql);
    while(query.next()){ //查询数据库是否已存在申请账号，如果有，则进行修改，否则进行添加
        if(query.value("phone").toString() == phone){
            sql = QString("select * from applicant where phone = '%1'").arg(phone);
            query.exec(sql);
            sql = QString("update applicant set profess = '%1',school = '%2',year = '%3',category = '%4',job = '%5' where phone = '%6'")
                    .arg(profess).arg(school).arg(year).arg(cate).arg(job).arg(phone);
            Ok = true;
            break;
        }
    }


    if(false == Ok){
        sql = QString("insert into applicant(name, phone, profess, school, year, category, job, sign, pass1, pass2, passHR) "
                       "values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11')")
                .arg(name).arg(phone).arg(profess).arg(school).arg(year).arg(cate).arg(job).arg("0").arg("0").arg("0").arg("0");
    }
    query.exec(sql);
    QMessageBox::about(this,"成功","提交成功！");
    ui->stackedWidget->setCurrentWidget(ui->pageUserHome);
    initHome();
    qDebug() << "提交成功";
}
//Home -> Main
void apply::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}
//修改
void apply::on_btnModify_clicked()
{
    QSqlQuery query;
    QString sql = QString("select modifyEN from boss where id = 1");
    query.exec(sql);
    while(query.next()){
        if(0 == query.value("modifyEN").toInt()){
           QMessageBox::warning(this,"通知","已经截止进行修改",QMessageBox::Yes);
           return;
        }
    }
    ui->stackedWidget->setCurrentWidget(ui->pageJobs);
    sql = QString("select * from applicant where phone = '%1'").arg(myPhone);
    query.exec(sql);
    query.next();
    {
        ui->lineEditProfess->setText(query.value("profess").toString());
        ui->lineEditSchool->setText(query.value("school").toString());
    }
}

//进行签到
void apply::on_btnSign_clicked()
{
    QSqlQuery query;
    QString sql = QString("select * from applicant where phone = '%1'").arg(ui->lineEditPhone_H->text());
    query.exec(sql);
    query.next();
    if(query.value("sign").toInt() != 0){
        return;
    }
    if(QMessageBox::No == (QMessageBox::information(this,"确认","确定现在签到？",QMessageBox::Yes|QMessageBox::No))){
        return;
    }
    QString sessionSign;
    if(query.value("pass1").toInt() == 0){
        sessionSign = "1";
    }else if(query.value("pass2").toInt() == 0 ){
        if(query.value("pass1").toInt() == 1) sessionSign = "2";
        else ui->btnSign->setEnabled(false);
    }else if(query.value("passHR").toInt() == 0 && query.value("pass2").toInt() == 1){
        if(query.value("pass1").toInt() == 1) sessionSign = "3";
        else ui->btnSign->setEnabled(false);
    }else{
        QMessageBox::about(this,"恭喜","你已经是公司的正式员工啦！\n还在这里签到干嘛");
        return;
    }
    sql = QString("update applicant set sign = '%1' where phone = '%2'").arg(sessionSign).arg(ui->lineEditPhone_H->text());
    query.exec(sql);
    ui->labelStatusSign->setText(signStatus);
    ui->labelPicSign->setPixmap(QPixmap(":/image/image/sign.png").scaled(ui->labelPicSign->size()));
    ui->btnSign->setEnabled(false);
}
//退出
void apply::on_btnExit_clicked()
{
    emit changeToLoginA("apply");
}
