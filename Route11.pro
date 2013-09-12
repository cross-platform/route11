TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp

HEADERS += \
    Circuits.h \
    Components.h \
    R11Component.h \
    R11ComponentLoop.h \
    R11ComponentPair.h \
    R11DspComponent.h \
    Route11.h

INCLUDEPATH += "/home/marcus/Projects/DSPatch/include"

debug:LIBS += -L/home/marcus/Projects/personal/DSPatch/build-DSPatch-Desktop_clang-Debug -lDSPatch
release:LIBS += -L/home/marcus/Projects/personal/DSPatch/build-DSPatch-Desktop_clang-Release -lDSPatch

LIBS += -lpthread
