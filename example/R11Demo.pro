TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    rtaudio/RtAudio.cpp \
    main.cpp \
    R11PpAudioDevice.cpp \
    R11PpWaveStreamer.cpp

HEADERS += \
    rtaudio/RtAudio.h \
    ../include/Route11.h \
    ../src/R11ProcessPair.h \
    ../src/R11ProcessLoop.h \
    ../src/R11Process.h \
    ../src/R11DspComponent.h \
    ../src/R11AsyncProcessThread.h \
    ../src/R11AsyncProcess.h \
    R11PsMicWaveMixer.h \
    R11PpWaveStreamer.h \
    R11PpAudioDevice.h \
    R11PpCrossfader.h

DEFINES += __LINUX_ALSA__
LIBS += -lasound
