#include "login.h"
#include <QApplication>
#include "apply.h"
#include "viewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
//    apply applyWin;
//    applyWin.setFixedSize(900,580);
//    applyWin.show();
//    viewer v;
//    v.show();

    return a.exec();
}
