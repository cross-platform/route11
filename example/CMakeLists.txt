project(Route11Example)

file(GLOB srcs *.cpp rtaudio/*.cpp)
file(GLOB hdrs *.h rtaudio/*.h)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/rtaudio
)

# Add definition of example audio file
add_definitions(-DEXAMPLE_WAV_FILE="${CMAKE_CURRENT_SOURCE_DIR}/Sample.wav")

add_executable(
    ${PROJECT_NAME}
    ${srcs}
    ${hdrs}
)

target_link_libraries(
    ${PROJECT_NAME}
    Route11
)

# Definition for RtAudio Windows, using WASAPI
if(WIN32)
    add_definitions(-D__WINDOWS_WASAPI__)

    target_link_libraries(
        ${PROJECT_NAME}
        ksuser
        mfplat
        mfuuid
        wmcodecdspuuid
    )
endif(WIN32)

# Definition for RtAudio Linux, using ALSA
if(UNIX AND NOT APPLE)
    add_definitions(-D__LINUX_ALSA__)

    find_library(ASOUND asound)
    if(NOT ASOUND)
        message(FATAL_ERROR "ALSA not found (Ensure that libasound2-dev is installed)")
    endif()

    target_link_libraries(
        ${PROJECT_NAME}
        ${ASOUND}
    )
endif(UNIX AND NOT APPLE)

# Definition for RtAudio Mac OSX, using Core Audio
if(APPLE)
    add_definitions(-D__MACOSX_CORE__)

    target_link_libraries(
        ${PROJECT_NAME}
        "-framework CoreAudio"
        "-framework CoreFoundation"
    )
endif(APPLE)
