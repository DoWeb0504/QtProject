#ifndef PUBFUNS_H
#define PUBFUNS_H
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QNetworkConfigurationManager>

void openDatabase();
void linkDatabase(QSqlQuery query,QString sql);


#endif // PUBFUNS_H
