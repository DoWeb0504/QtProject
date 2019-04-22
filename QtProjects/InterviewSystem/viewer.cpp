#include "viewer.h"
#include "ui_viewer.h"
#include "pubfuns.h"  //连接数据库函数
#include "pie.h"  //圆形统计图
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QModelIndexList>
#include <QPixmap>
viewer::viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::viewer)
{
    ui->setupUi(this);
    noticed = "已通知";
    absent = "缺席";
    finished = "面试完成";
    doing = "正在面试";
    passSta = "通过";
    unPassSta = "不通过";
    unDeside = "未表决";
    deside = "已表决";
    ui->stackedWidget->setCurrentWidget(ui->pageHome);
    setFixedSize(820,560);
    setWindowTitle("面试");

    //设置背景
    QPixmap pixmap = QPixmap(":/image/image/viewer.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);

    //rgb(85,170,255)
    QString button_style = "QPushButton{background-color:rgb(78,238,148);\
                            color:white;border-radius:10px;border:2px groove gray;\
                            border-style:outset;}"
                            "QPushButton:hover{background-color:white;color:black}"
                            "QPushButton:pressd{background-color:green;\
                                border-style:inset;}";  //按键样式表

    ui->btnAbsent->setStyleSheet(button_style);
    ui->btnDecToView->setStyleSheet(button_style);
    ui->btnEnd->setStyleSheet(button_style);
    ui->btnNotice->setStyleSheet(button_style);
    ui->btnPass->setStyleSheet(button_style);
    ui->btnStart->setStyleSheet(button_style);
    ui->btnToData->setStyleSheet(button_style);
    ui->btnToView->setStyleSheet(button_style);
    ui->btnunPass->setStyleSheet(button_style);
    ui->btnViewToDec->setStyleSheet(button_style);
    ui->btnViewToHome->setStyleSheet(button_style);
    ui->pushButton->setStyleSheet(button_style);
    ui->btnDataToHome->setStyleSheet(button_style);

    QString comBox_style = "QComboBox{border:1px solid #d7d7d7; border-radius: 3px; padding: 1px 18px 1px 3px;}"
                           "QComboBox:editable{ background: blue; }"
                           "QComboBox:!editable{ background: #7AC5CD; color:#FFFFFF}"
                           "QComboBox::drop-down{ subcontrol-origin: padding; subcontrol-position: top right; width: 22px; border-left-width: 1px;\
                           border-left-color: #c9c9c9; border-left-style: solid; /* just a single line */ border-top-right-radius: 3px; /* same radius as the QComboBox */border-bottom-right-radius: 3px; }"
                           "QComboBox::down-arrow:on { /* shift the arrow when popup is open */ top: 1px; left: 1px;}"
                           "QComboBox QAbstractItemView::item{max-width: 30px;min-height: 20px}";
    ui->comboBoxCate_H->setStyleSheet(comBox_style);
    ui->comboBoxJob_H->setStyleSheet(comBox_style);
    ui->comboBoxSession->setStyleSheet(comBox_style);

    ui->comboBoxCate_D->setStyleSheet(comBox_style);
    ui->comboBoxJob_D->setStyleSheet(comBox_style);
    ui->comboBoxSession_D->setStyleSheet(comBox_style);

//    openDatabase();
    initData();
    timer = new QTimer(this);


    //将interView的计时拆送给applicantView
    void (InterView::*funSignal)(QString) = &InterView::timeoutT;
    connect(&interView,funSignal,&appliView,&applicantView::dealTimeUpdate);
    connect(timer,&QTimer::timeout,this,&viewer::updateFunc);  //5s更新一次modelV
}

void viewer::updateFunc()  //刷新viewtable
{
    updateModelV();
    updateModelD();
}

viewer::~viewer()
{
    delete ui;
}
void viewer::initData()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);  //新建模型
    model->setQuery("select name from category");
    ui->comboBoxCate_H->setModel(model);  //将模型放入岗位类别下拉框
    ui->comboBoxCate_D->setModel(model);  //数据统计页面
    QSqlQueryModel *modelSession = new QSqlQueryModel(this);
    modelSession->setQuery("select session from category where id < 5");
    ui->comboBoxSession->setModel(modelSession);  //场次下拉框
    ui->comboBoxSession_D->setModel(modelSession);  //数据统计界面

    tableV = new QSqlTableModel(this);

    tableD = new QSqlTableModel(this);


}

//进行面试按键
void viewer::on_btnToView_clicked()
{
    if(ui->comboBoxSession->currentText() == "请选择场次"){
        QMessageBox::warning(this,"选择","请选择场次");
        return;
    }
    if(ui->comboBoxCate_H->currentText() == "请选择岗位类别"){
        QMessageBox::warning(this,"选择","请选择岗位类别");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->pageView);  //切换到view界面
    ui->labelSession->setText(ui->comboBoxSession->currentText()); //栏目显示  x面 类别 岗位
    ui->labelCate->setText(ui->comboBoxCate_H->currentText());
    ui->labelJob->setText(ui->comboBoxJob_H->currentText());
    ui->labelJob_D->setText(ui->comboBoxJob_H->currentText());
    ui->labelSession_D->setText(ui->comboBoxSession->currentText());
    timer->start(5000);
    updateModelV();
}
//更新modelV
void viewer::updateModelV(){
    QString sessionN;
    QString session;
    QString sessionLable = ui->labelSession->text();
    if(sessionLable == "一面"){
        sessionN = "1";
        session = "pass1";
    }else if(sessionLable == "二面"){
        sessionN = "2";
        session = "pass2";
    }else if(sessionLable == "HR面"){
        sessionN = "3";
        session = "passHR";
    }
    tableV->setTable("applicant");  //选择数据表
    ui->tableViewV->setModel(tableV);  //绑定模型
    QString sql = QString("(sign = '%1' or %2 = '1' or %2 = '2')and job = '%3'").arg(sessionN).arg(session).arg(ui->comboBoxJob_H->currentText());  //已签到的岗位
    tableV->setFilter(sql);  //放置过滤器
    tableV->select();  //显示%1
    for(int i = 5;i < 11;i++){
        ui->tableViewV->setColumnHidden(i,true);//隐藏
    }
    if(sessionLable == "一面"){
        for(int i=12;i<17;i++){
            ui->tableViewV->setColumnHidden(i,true);//隐藏
        }
    }else if(sessionLable == "二面"){
        ui->tableViewV->setColumnHidden(11,true);//隐藏
        ui->tableViewV->setColumnHidden(12,true);//隐藏
        for(int i=14;i<17;i++){
            ui->tableViewV->setColumnHidden(i,true);//隐藏
        }
    }else if(sessionLable == "HR面"){
        for(int i=11;i<15;i++){
            ui->tableViewV->setColumnHidden(i,true);//隐藏
        }
        ui->tableViewV->setColumnHidden(16,true);//隐藏
    }

    ui->tableViewV->setColumnHidden(12,true);//隐藏
    tableV->setHeaderData(0,Qt::Horizontal,"姓名");
    tableV->setHeaderData(1,Qt::Horizontal,"联系方式");
    tableV->setHeaderData(2,Qt::Horizontal,"所学专业");
    tableV->setHeaderData(3,Qt::Horizontal,"毕业院校");
    tableV->setHeaderData(4,Qt::Horizontal,"毕业年份");
    for(int i = 11;i < 16;i += 2){
        tableV->setHeaderData(i,Qt::Horizontal,"状态");
    }
    ui->tableViewV->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewV->setSelectionBehavior(QAbstractItemView::SelectRows);//单击选中整行
    ui->tableViewV->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许修改
//    ui->tableViewV->horizontalHeader()->setDefaultSectionSize(80); //设置默认列宽
    ui->tableViewV->resizeColumnsToContents(); //根据内容自动调整列宽
}

//更新modelD
void viewer::updateModelD()
{
    QString session;
    QString sessionLable = ui->labelSession->text();
    if(sessionLable == "一面"){
        session = "pass1";
    }else if(sessionLable == "二面"){
        session = "pass2";
    }else if(sessionLable == "HR面"){
        session = "passHR";
    }
    tableD->setTable("applicant");  //选择数据表
    ui->tableViewD->setModel(tableD);  //绑定模型
    QString sql = QString("((%1 = '6')or(%1 = '1')or(%1 = '2'))and job = '%2'").arg(session).arg(ui->comboBoxJob_H->currentText());
    tableD->setFilter(sql);  //放置过滤器
    tableD->select(); //显示
    for(int i = 5;i < 12;i++){
        ui->tableViewD->setColumnHidden(i,true);//隐藏
    }
    if(sessionLable == "一面"){
        for(int i = 13;i < 17;i++){
            ui->tableViewD->setColumnHidden(i,true);//隐藏
        }
    }else if(sessionLable == "二面"){
        ui->tableViewD->setColumnHidden(12,true);//隐藏
        ui->tableViewD->setColumnHidden(13,true);//隐藏
        ui->tableViewD->setColumnHidden(15,true);//隐藏
        ui->tableViewD->setColumnHidden(16,true);//隐藏
    }else if(sessionLable == "HR面"){
        for(int i = 12;i < 16;i++){
            ui->tableViewD->setColumnHidden(i,true);//隐藏
        }
    }

    tableD->setHeaderData(0,Qt::Horizontal,"姓名");
    tableD->setHeaderData(1,Qt::Horizontal,"联系方式");
    tableD->setHeaderData(2,Qt::Horizontal,"所学专业");
    tableD->setHeaderData(3,Qt::Horizontal,"毕业院校");
    tableD->setHeaderData(4,Qt::Horizontal,"毕业年份");
    for(int i = 12;i < 17;i += 2){
        tableD->setHeaderData(i,Qt::Horizontal,"通过状态");
    }

    ui->tableViewD->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewD->setSelectionBehavior(QAbstractItemView::SelectRows);//单击选中整行
    ui->tableViewD->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许修改
//    ui->tableViewD->horizontalHeader()->setDefaultSectionSize(80); //设置默认列宽
    ui->tableViewD->resizeColumnsToContents(); //根据内容自动调整列宽
}

//“查看面试”->“Home”
void viewer::on_btnViewToHome_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageHome);
    timer->stop();//关闭定时器
}
//"表决"->"查看面试"
void viewer::on_btnDecToView_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageView);
}
//“查看面试”->"表决"
void viewer::on_btnViewToDec_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageDecision);
}
//开始面试
void viewer::on_btnStart_clicked()
{
    int curRow = -1;
    curRow = ui->tableViewV->currentIndex().row();  //获取被选中的行
    qDebug() << curRow;
    if(curRow == -1) {
        QMessageBox::warning(this,"未选中","请选择面试对象");
        return;
    }
    QAbstractItemModel *modelV = ui->tableViewV->model();
    QModelIndex index = modelV->index(curRow,1);  //选中联系方式内容
    QVariant data = modelV->data(index); //提取电话号码
    qDebug() << data.toString();
    QSqlQuery query;
    QString session;
    QString status;
    if(ui->labelSession->text() == "一面"){
        session = "pass1";
        status = "status1";
    }else if(ui->labelSession->text() == "二面"){
        session = "pass2";
        status = "status2";
    }else{
        session = "passHR";
        status = "statusHR";
    }  //选中面试场次
    QString sql = QString("select %1 from applicant where phone = '%2'").arg(session).arg(data.toString());
    query.exec(sql);
    query.next();
    if(query.value(0) != "3" &&query.value(0) != "4"){
        qDebug() << "无法执行，返回";
        return;  //只有”已通知“和”缺勤“才可以往下执行
    }


        //两弹窗lable显示
        index = modelV->index(curRow,0);
        appliView.myName = modelV->data(index).toString();
        interView.myName = modelV->data(index).toString();

        index = modelV->index(curRow,1);
        appliView.myPhone = modelV->data(index).toString();
        interView.myPhone = modelV->data(index).toString();

        index = modelV->index(curRow,2);
        appliView.myProfess = modelV->data(index).toString();
        interView.myProfess = modelV->data(index).toString();

        index = modelV->index(curRow,3);
        appliView.mySchool = modelV->data(index).toString();
        interView.mySchool = modelV->data(index).toString();

        index = modelV->index(curRow,4);
        appliView.myYear = modelV->data(index).toString();
        interView.myYear = modelV->data(index).toString();

        index = modelV->index(curRow,6);
        appliView.myJob = modelV->data(index).toString();
        interView.myJob = modelV->data(index).toString();


    interView.show();
    appliView.show();
    appliView.dealShow();
    interView.dealShow();


//    ui->tableViewV->setCurrentIndex(NULL);
//    curRow = ui->tableViewV->currentIndex().row();
//    QAbstractItemModel *modelV = ui->tableViewV->model();
//    QModelIndex index;

    sql = QString("update applicant set %1 = '%2', %3 = '%4' where phone = '%5'")
            .arg(status).arg(doing).arg(session).arg("5").arg(data.toString());  //更新面试状态
    query.exec(sql);  //5:正在面试
    updateModelV();

}
//结束面试
void viewer::on_btnEnd_clicked()
{
    int curRow = ui->tableViewV->currentIndex().row();
    QAbstractItemModel *modelV = ui->tableViewV->model();
    QModelIndex index = modelV->index(curRow,1);  //选中联系方式内容
    QVariant data = modelV->data(index);
    qDebug() << data.toString();
    QSqlQuery query;
    QString session;
    QString status;
    QString desideSta;
    if(ui->labelSession->text() == "一面"){
        session = "pass1";
        status = "status1";
        desideSta = "desideSta1";
    }else if(ui->labelSession->text() == "二面"){
        session = "pass2";
        status = "status2";
        desideSta = "desideSta2";
    }else{
        session = "passHR";
        status = "statusHR";
        desideSta = "desideStaHR";
    }  //选中面试场次
    QString sql = QString("select %1 from applicant where phone = '%2'").arg(session).arg(data.toString());
    query.exec(sql);
    query.next();
    if(query.value(0) != "5"){
        qDebug() << "无法执行，返回";
        return;  //只有”正在面试“才可以执行
    }
    sql = QString("update applicant set %1 = '%2', %7 = '%3', %4 = '%5' where phone = '%6'")
            .arg(status).arg(finished).arg(unDeside).arg(session).arg("6").arg(data.toString()).arg(desideSta);  //更新面试状态
    query.exec(sql);  //6:完成面试
    qDebug() << "完成面试";
    updateModelV();

    /*************
     * 待开发：上传面试笔记到数据库
     * *************/

//    interView.close();
//    appliView.close();
}
//岗位种类下拉框
void viewer::on_comboBoxCate_H_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "请选择岗位类别"){
        ui->comboBoxJob_H->clear();
        ui->labelApplyNum->clear();
    }else{
        ui->comboBoxJob_H->clear();
        QSqlQuery query;
        QString sql = QString("select job from jobs where category = '%1'").arg(arg1);
        query.exec(sql);
        while(query.next()){
            QString name = query.value(0).toString();
            ui->comboBoxJob_H->addItem(name);
        }
    }
}
//岗位下拉框
void viewer::on_comboBoxJob_H_currentIndexChanged(const QString &arg1)
{
    int applyNum = 0;
    QSqlQuery query;
    QString sql = QString("select name from applicant where category = '%1' and job = '%2'")
            .arg(ui->comboBoxCate_H->currentText())
            .arg(arg1);
    query.exec(sql);
    while(query.next()){
        applyNum++;
    }
   ui->labelApplyNum->setText(QString::number(applyNum));
}
//通知面试
void viewer::on_btnNotice_clicked()
{
    int curRow = ui->tableViewV->currentIndex().row();  //获取选中行
    QAbstractItemModel *modelV = ui->tableViewV->model();
    QModelIndex index = modelV->index(curRow,1);  //选中联系方式内容
    QVariant data = modelV->data(index);
    qDebug() << data.toString();
    QSqlQuery query;
    QString session;
    QString status;
    if(ui->labelSession->text() == "一面"){
        session = "pass1";
        status = "status1";
    }else if(ui->labelSession->text() == "二面"){
        session = "pass2";
        status = "status2";
    }else{
        session = "passHR";
        status = "statusHR";
    }  //选中面试场次
    qDebug() << session;
    QString sql = QString("select %1 from applicant where phone = '%2'").arg(session).arg(data.toString());
    query.exec(sql);
    query.next();
    if(query.value(0) != "0" &&query.value(0) != "4"){
        qDebug() << "无法执行，返回";
        return;  //只有”未完成“和”缺勤“才可以往下执行
    }
    qDebug() << "允许执行";
    sql = QString("update applicant set %1 = '%2', %3 = '%4' where phone = '%5'")
            .arg(status).arg(noticed).arg(session).arg("3").arg(data.toString());  //更新面试状态
    query.exec(sql);  //3:通知面试
    updateModelV();
}
//缺席
void viewer::on_btnAbsent_clicked()
{
    int curRow = ui->tableViewV->currentIndex().row();
    QAbstractItemModel *modelV = ui->tableViewV->model();
    QModelIndex index = modelV->index(curRow,1);  //选中联系方式内容
    QVariant data = modelV->data(index);
    qDebug() << data.toString();
    QSqlQuery query;
    QString session;
    QString status;
    if(ui->labelSession->text() == "一面"){
        session = "pass1";
        status = "status1";
    }else if(ui->labelSession->text() == "二面"){
        session = "pass2";
        status = "status2";
    }else{
        session = "passHR";
        status = "statusHR";
    }  //选中面试场次
    QString sql = QString("select %1 from applicant where phone = '%2'").arg(session).arg(data.toString());
    query.exec(sql);
    query.next();
    if(query.value(0) != "3"){
        qDebug() << "无法执行，返回";
        return;  //只有”未完成“和”缺勤“才可以往下执行
    }
    qDebug() << "允许执行";
    sql = QString("update applicant set %1 = '%2', %3 = '%4' where phone = '%5'")
            .arg(status).arg(absent).arg(session).arg("4").arg(data.toString());  //更新面试状态
    query.exec(sql);  //4：缺席
    updateModelV();
}
//通过
void viewer::on_btnPass_clicked()
{
    int curRow = ui->tableViewD->currentIndex().row();
    QAbstractItemModel *modelV = ui->tableViewD->model();
    QModelIndex index = modelV->index(curRow,1);  //选中联系方式内容
    QVariant data = modelV->data(index);
    qDebug() << data.toString();
    QSqlQuery query;
    QString session;
    QString status;
    QString desideStatus;
    if(ui->labelSession->text() == "一面"){
        session = "pass1";
        status = "status1";
        desideStatus = "desideSta1";
    }else if(ui->labelSession->text() == "二面"){
        session = "pass2";
        status = "status2";
        desideStatus = "desideSta2";
    }else{
        session = "passHR";
        status = "statusHR";
        desideStatus = "desideStaHR";
    }  //选中面试场次

    QString sql = QString("update applicant set %1 = '%2', %3 = '%4',%5 = '%6', sign = '0' where phone = '%7'")
            .arg(desideStatus).arg(passSta).arg(session).arg("1").arg(status).arg(deside).arg(data.toString());  //更新面试状态
    query.exec(sql);  //1：通过
    updateModelD();

}
//不通过
void viewer::on_btnunPass_clicked()
{
    int curRow = ui->tableViewD->currentIndex().row();
    QAbstractItemModel *modelV = ui->tableViewD->model();
    QModelIndex index = modelV->index(curRow,1);  //选中联系方式内容
    QVariant data = modelV->data(index);
    qDebug() << data.toString();
    QSqlQuery query;
    QString session;
    QString status;
    QString desideStatus;
    if(ui->labelSession->text() == "一面"){
        session = "pass1";
        status = "status1";
        desideStatus = "desideSta1";
    }else if(ui->labelSession->text() == "二面"){
        session = "pass2";
        status = "status2";
        desideStatus = "desideSta2";
    }else{
        session = "passHR";
        status = "statusHR";
        desideStatus = "desideStaHR";
    }  //选中面试场次
    QString sql = QString("update applicant set %1 = '%2', %3 = '%4', %5 = '%6', sign = '0' where phone = '%7'")
            .arg(desideStatus).arg(unPassSta).arg(session).arg("2").arg(status).arg(deside).arg(data.toString());  //更新面试状态
    query.exec(sql);  //2：不通过
    updateModelD();
}
//退出
void viewer::on_pushButton_clicked()
{
    emit changeToLoginV("viewer");
}


/*************查看数据*************/
//返回按键
void viewer::on_btnDataToHome_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageHome);
}
//查看数据按键
void viewer::on_btnToData_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageDataView);
//    ui->stackedWidget_D->setCurrentWidget(ui->pageMain);  //显示主页面
}
//类别下拉框
void viewer::on_comboBoxCate_D_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "请选择岗位类别"){
        ui->comboBoxJob_D->clear();
        clearLableD();
    }else{
        ui->comboBoxJob_D->clear();
        QSqlQuery query;
        QString sql = QString("select job from jobs where category = '%1'").arg(arg1);
        query.exec(sql);
        while(query.next()){
            QString name = query.value(0).toString();
            ui->comboBoxJob_D->addItem(name);
        }
    }
}
//岗位下拉框
void viewer::on_comboBoxJob_D_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBoxSession_D->currentText() == "请选择场次"){
        ui->stackedWidget_D->setCurrentWidget(ui->pageMain);
        showpageMain(arg1);
    }else{
        clearLableD();
        ui->stackedWidget_D->setCurrentWidget(ui->pageData);
        //场次显示部分
        showpageData(ui->comboBoxJob_D->currentText());
    }
}

//场次下拉框
void viewer::on_comboBoxSession_D_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBoxJob_D->currentText() == NULL) return;
    if(arg1 == "请选择场次"){
        clearLableD();
        ui->stackedWidget_D->setCurrentWidget(ui->pageMain);
        showpageMain(ui->comboBoxJob_D->currentText());
    }else{
        clearLableD();
        showpageData(arg1);
    }
}

//pageMain界面总显示
void viewer::showpageMain(const QString &arg1){
    int deliver = 0;
    int pass1 = 0;
    int pass2 = 0;
    int passHR = 0;
    QSqlQuery query;
    QString sql = QString("select pass1,pass2,passHR from applicant where category = '%1' and job = '%2'")
            .arg(ui->comboBoxCate_D->currentText())
            .arg(arg1);
    query.exec(sql);
    while(query.next()){
        if(query.value("pass2") == 1) ++pass2;
        if(query.value("pass1") == 1) ++pass1;
        if(query.value("passHR") == 1) ++passHR;
        ++deliver;
    }
    ui->labelDelivery_D->setText(QString::number(deliver));
    ui->labelPass1_D->setText(QString::number(pass1));
    ui->labelPass2_D->setText(QString::number(pass2));
    ui->labelPassHR_D->setText(QString::number(passHR));
    pass2 = pass2 - passHR;
    pass1 = pass1 - pass2 - passHR;
    float deliverF = QString::number(deliver).toFloat();
    float passHRF = QString::number(passHR).toFloat();
    float pass2F = QString::number(pass2).toFloat();
    float pass1F = QString::number(pass1).toFloat();
//    qDebug() << deliverF << " " << pass1F << " " << pass2F << " " << passHRF <<endl;


    QVector<SectorInfo> result;
    SectorInfo info;
    info.description = "通过一面";
    info.percent = (pass1F / deliverF)*100;
    result.push_back(info);
    info.description = "通过二面";
    info.percent = (pass2F / deliverF)*100;
    result.push_back(info);
    info.description = "通过HR面";
    info.percent = (passHRF / deliverF)*100;
    result.push_back(info);
    info.description = "其他";
    info.percent = ((deliverF - pass1F - pass2F - passHRF) / deliverF)*100;
    result.push_back(info);
    ui->pie->setData(result);
}

//数统计界面数据清除
void viewer::clearLableD(){
    ui->labelDelivery_D->clear();
    ui->labelPass1_D->clear();
    ui->labelPass2_D->clear();
    ui->labelPassHR_D->clear();
    ui->labelNotDec_D->clear();
    ui->labelNotPass_D->clear();
    ui->labelNotView_D->clear();
    ui->labelPass_D->clear();
    QVector<SectorInfo> result;
    ui->pie->setData(result);  //清空圆饼
}
//pageData界面显示
void viewer::showpageData(const QString &arg1){
    ui->stackedWidget_D->setCurrentWidget(ui->pageData);
    int sum = 0;
    int notDec = 0;
    int passNum = 0;
    int notPass = 0;
    if(arg1 == "一面"){

        QSqlQuery query;
        QString sql = QString("select pass1 from applicant where job = '%1'")
                .arg(ui->comboBoxJob_D->currentText());
        query.exec(sql);
        while(query.next()){
            ++sum;
            int sta = query.value("pass1").toInt();
            if(sta == 1) ++passNum;
            else if(sta == 2) ++notPass;
            else if(sta == 5 || sta == 6) ++notDec;
        }

    }else{
        QString pass;
        QString lastPass;
        if(arg1 == "二面"){
            pass = "pass2";
            lastPass = "pass1";
        }else{
            pass = "passHR";
            lastPass = "pass2";
        }
        QSqlQuery query;
        QString sql = QString("select %1,%2 from applicant where job = '%3'")
                .arg(lastPass).arg(pass).arg(ui->comboBoxJob_D->currentText());
        query.exec(sql);
        while(query.next()){
            if(query.value(lastPass) == 1) ++sum;
            int sta = query.value(pass).toInt();
            if(sta == 1) ++passNum;
            else if(sta == 2) ++notPass;
            else if(sta == 5 || sta == 6) ++notDec;
        }
    }
    int notView = sum - passNum - notDec - notPass;
    ui->labelNotView_D->setText(QString::number(notView));
    ui->labelNotDec_D->setText(QString::number(notDec));
    ui->labelPass_D->setText(QString::number(passNum));
    ui->labelNotPass_D->setText(QString::number(notPass));

    float sumF = QString::number(sum).toFloat();
    float notDecF = QString::number(notDec).toFloat();
    float notPassF = QString::number(notPass).toFloat();
    float passF = QString::number(passNum).toFloat();
    float notViewF = QString::number(notView).toFloat();

    QVector<SectorInfo> result;
    SectorInfo info;
    info.description = "未面试";
    info.percent = (notViewF / sumF)*100;
    result.push_back(info);
    info.description = "未表决";
    info.percent = (notDecF / sumF)*100;
    result.push_back(info);
    info.description = "通过";
    info.percent = (passF / sumF)*100;
    result.push_back(info);
    info.description = "未通过";
    info.percent = (notPassF / sumF)*100;
    result.push_back(info);
    ui->pie->setData(result);
}
