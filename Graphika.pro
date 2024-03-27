QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    functionlayout.cpp \
    graphlayout.cpp \
    layoutfactory.cpp \
    main.cpp \
    mainwindow.cpp \
    polynomelayout.cpp \
    pyhelper.cpp \
    stringparser.cpp \
    toolbar.cpp \
    validatestring.cpp


HEADERS += \
    expressionvalidator.h \
    functionlayout.h \
    graphlayout.h \
    layoutfactory.h \
    mainwindow.h \
    polynomelayout.h \
    pyhelper.h \
    stringparser.h \
    toolbar.h \
    validatestring.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../media/mark/22000_318_C1/image.png \
    Window.qml
