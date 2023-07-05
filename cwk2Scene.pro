TEMPLATE = app
TARGET = Scene
INCLUDEPATH += . /opt/local/include

QT += widgets opengl gui

CONFIG += c++11

LIBS += -lGLU

HEADERS += SceneWidget.h \
           SceneWindow.h \
           stb_image.h

SOURCES += SceneMain.cpp \
           SceneWidget.cpp \
           SceneWindow.cpp
