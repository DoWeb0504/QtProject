#include "applicantview.h"
#include "ui_applicantview.h"
#include <QDebug>
#include "viewer.h"



applicantView::applicantView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::applicantView)
{
    ui->setupUi(this);
    setFixedSize(800,580);
    setWindowTitle("个人信息");


    //设置背景
    QPixmap pixmap = QPixmap(":/image/image/applicantview.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
}

applicantView::~applicantView()
{
    delete ui;
}
//显示计时
void applicantView::dealTimeUpdate(QString showStr){
    ui->lcdNumber->display(showStr);
}

void applicantView::dealShow(){  //初始化应聘生显示界面
    ui->labelName->setText(myName);
    ui->labelPhone->setText(myPhone);
    ui->labelJob->setText(myJob);
    ui->labelProfess->setText(myProfess);
    ui->labelSchool->setText(mySchool);
    ui->labelYear->setText(myYear);
    qDebug() << myName;
}
