#-------------------------------------------------
#
# Project created by QtCreator 2019-02-06T09:27:28
#
#-------------------------------------------------

QT       += core gui network widgets

lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5")

TARGET = x-innometrics
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
CONFIG += debug

LIBS += -lxcb -lxcb-ewmh -lxcb-icccm

SOURCES += \
        main.cpp        \
        ewmh.cpp        \
        networking.cpp  \
        mainwindow.cpp  \
	login_dialog.cpp \
	activities.cpp	\
        menubar.cpp

HEADERS += \
	ewmh.h		\
        networking.h    \
        mainwindow.h    \
	activities.h	\
	login_dialog.h	\
        menubar.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target