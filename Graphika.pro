QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    graphbuilder.cpp \
    main.cpp \
    mainwindow.cpp \
    stringparser.cpp \
    validatestring.cpp

INCLUDEPATH +="/usr/local/include/opencv4"

LIBS += -L"/usr/share/doc/"
LIBS += \
   -lopencv_core \
   -lopencv_features2d \
   -lopencv_highgui \
   -lopencv_imgcodecs \
   -lopencv_imgproc \
   -lopencv_video \
   -lopencv_videoio

HEADERS += \
    expressionvalidator.h \
    graphbuilder.h \
    mainwindow.h \
    stringparser.h \
    validatestring.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Window.qml
