#-------------------------------------------------
#
# Project created by QtCreator 2019-03-22T14:31:02
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = login
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    boss.cpp \
    apply.cpp \
    pubfuns.cpp \
    viewer.cpp \
    interview.cpp \
    applicantview.cpp \
    pie.cpp

HEADERS  += login.h \
    boss.h \
    apply.h \
    pubfuns.h \
    viewer.h \
    interview.h \
    applicantview.h \
    pie.h

FORMS    += login.ui \
    boss.ui \
    apply.ui \
    viewer.ui \
    interview.ui \
    applicantview.ui

RESOURCES += \
    res.qrc

RC_FILE = appLogo.rc

DISTFILES += \
    readme.txt
