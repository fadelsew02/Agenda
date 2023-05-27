QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSystemTrayIcon
TEMPLATE = app

CONFIG += c++17
CONFIG += debug
QT += sql
QT += core
QT += multimedia widgets


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    delete_events.cpp \
    dialog.cpp \
    events_save.cpp \
    main.cpp \
    calendar.cpp

HEADERS += \
    calendar.h \
    delete_events.h \
    dialog.h \
    events_save.h

FORMS += \
    calendar.ui \
    delete_events.ui \
    dialog.ui \
    events_save.ui

RC_ICONS = myappico.ico

QMAKE_CXXFLAGS += -std=gnu++14

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
