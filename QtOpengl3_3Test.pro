#-------------------------------------------------
#
# Project created by QtCreator 2015-12-04T10:09:12
#
#-------------------------------------------------

QT += widgets

TARGET = QtOpengl3_3Test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    refreshthread.cpp \
    posix_thread_wrapper/posixthreadwrapper.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    refreshthread.h \
    posix_thread_wrapper/posixthreadwrapper.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
