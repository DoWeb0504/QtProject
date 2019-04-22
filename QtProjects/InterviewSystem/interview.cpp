#include "interview.h"
#include "ui_interview.h"

InterView::InterView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InterView)
{
    ui->setupUi(this);
    setFixedSize(800,680);
    setWindowTitle("面试记录");
    QString button_style = "QPushButton{background-color:rgb(72,209,204);\
                            color:white;border-radius:10px;border:2px groove gray;\
                            border-style:outset;}"
                            "QPushButton:hover{background-color:white;color:black}"
                            "QPushButton:pressd{background-color:green;\
                             border-style:inset;}";  //按键样式表
    ui->btnEndTime->setStyleSheet(button_style);
    ui->btnStartTime->setStyleSheet(button_style);
    //设置背景
    QPixmap pixmap = QPixmap(":/image/image/interview.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);


    qtimer = new QTimer(this);
    ui->btnStartTime->setEnabled(true);
    ui->btnEndTime->setEnabled(false);

    connect(qtimer,&QTimer::timeout,this,&InterView::updateTime);
}

InterView::~InterView()
{
    delete ui;
}
//显示时间槽函数
void InterView::updateTime(){
    QTime current = QTime::currentTime();
    int tmp = baseTime.msecsTo(current);  //计算差值
    QTime showTimes(0,0,0,0);
    showTimes = showTimes.addMSecs(tmp);
    showStr = showTimes.toString("hh:mm:ss");  //时间转换成字符串
    ui->lcdNumber->display(showStr);
    emit timeoutT(showStr);
}
//初始化lable
void InterView::dealShow(){
    ui->labelName->setText(myName);
    ui->labelPhone->setText(myPhone);
    ui->labelJob->setText(myJob);
    ui->labelProfess->setText(myProfess);
    ui->labelSchool->setText(mySchool);
    ui->labelYear->setText(myYear);
}
//开始计时
void InterView::on_btnStartTime_clicked()
{
    baseTime = QTime::currentTime();
    qtimer->start(1000);
    ui->btnStartTime->setEnabled(false);
    ui->btnEndTime->setEnabled(true);
    ui->lcdNumber->display("00:00");
}
//结束计时
void InterView::on_btnEndTime_clicked()
{
    qtimer->stop();
    ui->btnEndTime->setEnabled(false);
    ui->btnStartTime->setEnabled(true);
}
