project(route11_example)

add_definitions(
    -D__LINUX_ALSA__
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/rtaudio
    ${CMAKE_SOURCE_DIR}/include
)

# Add definition of example audio file
add_definitions(-DEXAMPLE_WAV_FILE="${CMAKE_CURRENT_SOURCE_DIR}/Sample.wav")

add_executable(
    ${PROJECT_NAME}

    rtaudio/RtAudio.cpp
    rtaudio/RtAudio.h

    main.cpp

    R11PpAudioDevice.cpp
    R11PpAudioDevice.h
    R11PpCrossfader.h
    R11PpWaveStreamer.cpp
    R11PpWaveStreamer.h
    R11PsMicWaveMixer.h
)

target_link_libraries(
    ${PROJECT_NAME}

    route11

    asound
    pthread
)
