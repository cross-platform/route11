TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    example/main.cpp \
    example/R11WaveStreamer.cpp \
    example/R11AudioDevice.cpp \
    example/rtaudio/RtAudio.cpp

HEADERS += \
    example/R11WaveStreamer.h \
    example/R11Crossfader.h \
    example/R11AudioDevice.h \
    example/rtaudio/RtAudio.h \
    example/R11MicroWaveMixer.h

INCLUDEPATH += "src"

DEFINES +=__LINUX_ALSA__
LIBS += -lasound
