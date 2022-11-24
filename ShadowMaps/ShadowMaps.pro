QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
INCLUDEPATH += "include"
INCLUDEPATH += $$PWD

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    main.cpp \
    mainwindow.cpp \
    qpaintingwidget.cpp \
    resourcemanager.cpp \
    shadowmapwidget.cpp \
    simpledraw.cpp \
    texture2d.cpp \
    utils.cpp

HEADERS += \
    camera.h \
    color.h \
    glad.h \
    mainwindow.h \
    qpaintingwidget.h \
    resourcemanager.h \
    shader.h \
    shadowmapwidget.h \
    simpledraw.h \
    texture2d.h \
    utils.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#include glew/glfw
INCLUDEPATH += /usr/local/include

#lib glew / glfw.
macx: LIBS += -L/usr/local/lib/ -lGLEW.2.1.0 -lGLEW -lGLEW.2.1

#osx opengl framework
macx: LIBS+= -framework opengl -framework Cocoa -framework IOKit -framework CoreVideo

RESOURCES += \
    resources.qrc

DISTFILES +=

