#include "boss.h"
#include "ui_boss.h"
#include "pubfuns.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantList>
#include <QDebug>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QItemSelectionModel>
#include <QPixmap>

Boss::Boss(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Boss)
{
    ui->setupUi(this);
    setWindowTitle("ControlHome");
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
    setFixedSize(556,478);

    //设置背景
    QPixmap pixmap = QPixmap(":/image/image/boss.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);

    QString button_style = "QPushButton{background-color:rgb(85,170,255);\
                            color:white;border-radius:10px;border:2px groove gray;\
                            border-style:outset;}"
                            "QPushButton:hover{background-color:white;color:black}"
                            "QPushButton:pressd{background-color:blue;\
                                border-style:inset;}";  //按键样式表

    ui->btnAdd->setStyleSheet(button_style);
    ui->btnBack1->setStyleSheet(button_style);
    ui->btnDelete->setStyleSheet(button_style);
    ui->btnExit->setStyleSheet(button_style);
    ui->btnFind->setStyleSheet(button_style);
    ui->btnModifyEN->setStyleSheet(button_style);
    ui->btnSure->setStyleSheet(button_style);
    ui->btnView->setStyleSheet(button_style);
    ui->pushButton->setStyleSheet(button_style);

    openDatabase();
    //设置模型
    model = new QSqlTableModel(this);
    showTable();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); //手动提交修改
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许修改
    QSqlQuery query;
    QString sql = QString("select modifyEN from boss where id = 1");
    query.exec(sql);
    while(query.next()){  //显示提交申请控制按钮
        if(1 == query.value("modifyEN").toInt()){
            ui->btnModifyEN->setText("截止提交申请");
                return;
        }else{
            ui->btnModifyEN->setText("开放提交申请");
        }
    }
}

Boss::~Boss()
{
    delete ui;
}
//退出
void Boss::on_btnExit_clicked()
{
    emit changeToLoginB("boss");//发送信号给'login'
}
//转到面试官管理界面
void Boss::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageManage);
}
//返回主界面
void Boss::on_btnBack1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}
//增加
void Boss::on_btnAdd_clicked()
{
    QSqlRecord record = model->record();//获取空记录
    int row = model->rowCount();//获取行号
    model->insertRecord(row,record);

}
//删除
void Boss::on_btnDelete_clicked()
{
    if(QMessageBox::No == QMessageBox::information(this,"确认","确定删除选中？",QMessageBox::Yes|QMessageBox::No)){
        return;
    }
    QItemSelectionModel *sModel = ui->tableView->selectionModel();//获取选中的模型
    QModelIndexList list = sModel->selectedRows();//取出模型中的索引
    for(int i = 0; i < list.size(); i++){
        model->removeRow(list.at(i).row());
    }
}
//查找
void Boss::on_btnFind_clicked()
{
    QString findText = ui->lineEditFind->text();
    if(findText.length() == 0){
        //model->select();
        showTable();
        return;
    }
    QSqlQuery query;
    query.exec("select * from Inter");
    while(query.next()){
        if(query.value("name").toString() == findText){
            QString str = QString("name = '%1'").arg(findText);
            model->setFilter(str);
            model->select();
            return;
        }else if(query.value("num").toString() == findText){
            QString str = QString("num = '%1'").arg(findText);
            model->setFilter(str);
            model->select();
            return;
        }
    }
    QMessageBox::information(this,"Not","NOT FOUND!",QMessageBox::Ok);
}
//确定
void Boss::on_btnSure_clicked()
{

    if(QMessageBox::Yes == QMessageBox::information(this,"确认","确定更改？",QMessageBox::Yes|QMessageBox::No)){
        model->submitAll();//提交动作
    }else{
        model->revertAll(); //取消所有动作
        model->submitAll();  //提交动作
    }
}
//显示table
void Boss::showTable(){
    model->setTable("Inter");  //指定使用面试官表
    ui->tableView->setModel(model); //将model放置于tableView里
    model->select();//显示模型的数据
//    model->setFilter("num = '3116006913'");
    ui->tableView->setColumnHidden(0,true);//隐藏
    ui->tableView->setColumnHidden(3,true);
    model->setHeaderData(1,Qt::Horizontal,"姓名");
    model->setHeaderData(2,Qt::Horizontal,"工号");
}
//查看面试
void Boss::on_btnView_clicked()
{
    QMessageBox::about(this,"更新","😲老板，我不想开发啦，去注册个面试官账号查看数据统计吧😔");
}
//申请控制
void Boss::on_btnModifyEN_clicked()
{
    QSqlQuery query;
    QString sql = QString("select modifyEN from boss where id = 1");
    query.exec(sql);
    while(query.next()){
        if(1 == query.value("modifyEN").toInt()){
            if(QMessageBox::Yes == QMessageBox::warning(this,"确认","确定截至提交申请？",QMessageBox::Yes|QMessageBox::No)){
                sql = QString("update boss set modifyEN = 0");
                query.exec(sql);
                ui->btnModifyEN->setText("开放提交申请");
                return;
            }
        }else{
            if(QMessageBox::Yes == QMessageBox::warning(this,"确认","确定开放提交申请？",QMessageBox::Yes|QMessageBox::No)){
                sql = QString("update boss set modifyEN = 1");
                query.exec(sql);
                ui->btnModifyEN->setText("截止提交申请");
                return;
            }
        }
    }
}
