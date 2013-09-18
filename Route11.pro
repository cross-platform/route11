TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp

HEADERS += \
    R11DspComponent.h \
    Route11.h \
    R11Process.h \
    R11ProcessPair.h \
    R11ProcessLoop.h \
    Processes.h \
    Components.h \
    R11AsyncProcessThread.h \
    R11AsyncProcess.h

INCLUDEPATH += "/home/marcus/Projects/DSPatch/include"

debug:LIBS += -L/home/marcus/Projects/personal/DSPatch/build-DSPatch-Desktop_clang-Debug -lDSPatch
release:LIBS += -L/home/marcus/Projects/personal/DSPatch/build-DSPatch-Desktop_clang-Release -lDSPatch

LIBS += -lpthread
