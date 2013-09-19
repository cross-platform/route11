TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

HEADERS += \
    R11DspComponent.h \
    Route11.h \
    R11Process.h \
    R11ProcessPair.h \
    R11ProcessLoop.h \
    R11AsyncProcessThread.h \
    R11AsyncProcess.h
