TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

HEADERS += \
    src/R11DspComponent.h \
    src/Route11.h \
    src/R11Process.h \
    src/R11ProcessPair.h \
    src/R11ProcessLoop.h \
    src/R11AsyncProcessThread.h \
    src/R11AsyncProcess.h
