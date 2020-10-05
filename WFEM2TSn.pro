#-------------------------------------------------
#
# Project created by QtCreator 2020-09-26T09:24:54
#
#
# 将广域接收机采集到的数据转换成凤凰仪器的TSn格式
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WFEM2TSn
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Common/PublicFunction.cpp \
    TSnWork.cpp

HEADERS  += mainwindow.h \
    Common/PublicDef.h \
    Common/PublicFunction.h \
    TSnWork.h

FORMS    += mainwindow.ui
