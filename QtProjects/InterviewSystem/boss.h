#ifndef BOSS_H
#define BOSS_H

#include <QWidget>
#include <QSqlTableModel>


namespace Ui {
class Boss;
}

class Boss : public QWidget
{
    Q_OBJECT

public:
    explicit Boss(QWidget *parent = 0);
    ~Boss();

    void showTable();

signals:
    void changeToLoginB(QString);

private slots:
    void on_btnExit_clicked();

    void on_pushButton_clicked();

    void on_btnBack1_clicked();

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnFind_clicked();

    void on_btnSure_clicked();

    void on_btnView_clicked();

    void on_btnModifyEN_clicked();

private:
    Ui::Boss *ui;
    QSqlTableModel *model;
};

#endif // BOSS_H
