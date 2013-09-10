TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp

HEADERS += \
    Components.h \
    Route11.h \
    R11Component.h \
    Circuits.h \
    R11ComponentPair.h \
    R11ComponentLoop.h

Release:LIBS += "C:/Projects/Qt Projects/DSPatch/release/libDSPatch.a"
Debug:LIBS += "C:/Projects/Qt Projects/DSPatch/debug/libDSPatch.a"
