#ifndef APPLICANTVIEW_H
#define APPLICANTVIEW_H

#include <QWidget>

namespace Ui {
class applicantView;
}

class applicantView : public QWidget
{
    Q_OBJECT

public:
    explicit applicantView(QWidget *parent = 0);
    ~applicantView();

    void dealShow();
    void dealTimeUpdate(QString showStr);

public:
    QString myName;
    QString myPhone;
    QString myProfess;
    QString mySchool;
    QString myYear;
    QString myJob;

private slots:



private:
    Ui::applicantView *ui;

};

#endif // APPLICANTVIEW_H
