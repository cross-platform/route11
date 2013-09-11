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

Release:LIBS += "C:/Projects/Qt Projects/DSPatch/release/libDSPatch.a"
Debug:LIBS += "C:/Projects/Qt Projects/DSPatch/debug/libDSPatch.a"
