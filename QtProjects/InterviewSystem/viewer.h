#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include "interview.h"
#include "applicantview.h"
#include <QSqlTableModel>
#include <QTimer>

namespace Ui {
class viewer;
}

class viewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit viewer(QWidget *parent = 0);
    ~viewer();

    void initData();
    void updateModelV();
    void updateModelD();
    void updateFunc();
    void showpageMain(const QString &arg1);
    void clearLableD();
    void showpageData(const QString &arg1);

signals:
    void changeToLoginV(QString);

private slots:
    void on_btnToView_clicked();

    void on_btnViewToHome_clicked();

    void on_btnDecToView_clicked();

    void on_btnViewToDec_clicked();

    void on_btnStart_clicked();

    void on_btnEnd_clicked();

    void on_comboBoxCate_H_currentIndexChanged(const QString &arg1);

    void on_comboBoxJob_H_currentIndexChanged(const QString &arg1);

    void on_btnNotice_clicked();

    void on_btnAbsent_clicked();


    
    void on_btnPass_clicked();

    void on_btnunPass_clicked();

    void on_pushButton_clicked();

    void on_btnDataToHome_clicked();

    void on_btnToData_clicked();

    void on_comboBoxCate_D_currentIndexChanged(const QString &arg1);

    void on_comboBoxJob_D_currentIndexChanged(const QString &arg1);

    void on_comboBoxSession_D_currentIndexChanged(const QString &arg1);

private:
    Ui::viewer *ui;

    InterView interView;
    applicantView appliView;


    QSqlTableModel *tableV;
    QSqlTableModel *tableD;

    QString noticed;
    QString absent;
    QString finished;
    QString doing;
    QString passSta;
    QString unPassSta;
    QString unDeside;
    QString deside;

    QTimer *timer;
};

#endif // VIEWER_H
