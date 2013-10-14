cmake_minimum_required(VERSION 2.8)

project(route11)

add_definitions(
    -std=c++11
    -fno-permissive
    -pedantic

    -Wall
    -Wextra

    -D__LINUX_ALSA__
)

add_executable(
    ${PROJECT_NAME}

    ./rtaudio/RtAudio.cpp
    ./rtaudio/RtAudio.h

    ./main.cpp

    ./R11PpAudioDevice.cpp
    ./R11PpAudioDevice.h
    ./R11PpCrossfader.h
    ./R11PpWaveStreamer.cpp
    ./R11PpWaveStreamer.h
    ./R11PsMicWaveMixer.h

    ../include/Route11.h

    ../src/R11AsyncProcess.h
    ../src/R11AsyncProcessThread.h
    ../src/R11DspComponent.h
    ../src/R11Process.h
    ../src/R11ProcessLoop.h
    ../src/R11ProcessPair.h
)

target_link_libraries(
    ${PROJECT_NAME}

    asound
    pthread
)
