TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    main.cpp \
    R11WaveStreamer.cpp \
    R11AudioDevice.cpp \
    rtaudio/RtAudio.cpp

HEADERS += \
    R11WaveStreamer.h \
    R11Crossfader.h \
    R11AudioDevice.h \
    rtaudio/RtAudio.h \
    R11MicroWaveMixer.h \
    ../include/Route11.h \
    ../src/R11ProcessPair.h \
    ../src/R11ProcessLoop.h \
    ../src/R11Process.h \
    ../src/R11DspComponent.h \
    ../src/R11AsyncProcessThread.h \
    ../src/R11AsyncProcess.h

DEFINES +=__LINUX_ALSA__
LIBS += -lasound
