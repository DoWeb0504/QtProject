#include "pubfuns.h"

void linkDatabase(QSqlQuery query,QString sql)
{
    int times;

    while(!query.exec(sql)){
        if(times++ > 3) return;
        if(QMessageBox::Discard == QMessageBox::warning(NULL,"无网络","当前无网络，请确保已连接网络",QMessageBox::Yes|QMessageBox::Discard)){
            return;
        }
        openDatabase();
    }
    qDebug() << "连接成功！";
}


void openDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//    QSqlDatabase db1 = QSqlDatabase::addDatabase("QMYSQL","a"); //多个数据库的别名
    //连接数据库
    db.setHostName("47.107.154.232"); //数据库服务器IP
    db.setPort(3306);
    db.setUserName("root"); //数据库用户名
    db.setPassword("5d0e004240"); //密码  (使用服务器密码)
    db.setDatabaseName("interview"); //使用的数据库
    if(!db.open()){  //数据库打开失败
        QMessageBox::warning(NULL,"错误",db.lastError().text());
        qDebug() << "ERROR" << endl;
        qDebug() << db.lastError().text();
        return;
    }
    qDebug() << "Ok";
}


