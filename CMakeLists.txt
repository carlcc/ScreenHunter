cmake_minimum_required(VERSION 3.2)

project(PlayBlend2D)

set(CMAKE_CXX_STANDARD 14)


file(GLOB EXAMPLE src/example/*)
file(GLOB EXAMPLE_PATH src/example)

link_directories(
        3rdparty/lib
)


foreach (proj ${EXAMPLE})
    file(GLOB_RECURSE proj_src ${proj}/*)
    string(REPLACE "${EXAMPLE_PATH}/" "" out_name ${proj})
    message(${out_name})
    add_executable(${out_name} ${proj_src} )
    target_include_directories(${out_name} PRIVATE 3rdparty/include)
    target_link_libraries(${out_name} blend2d)
endforeach (proj)

if (APPLE)
    find_library(avfoundation_lib AVFoundation REQUIRED)
    find_library(coregraphics_lib CoreGraphics REQUIRED)
    find_library(corevideo_lib CoreVideo REQUIRED)
    find_library(cocoa_lib Cocoa REQUIRED)
    find_library(iokit_lib IOKit REQUIRED)
    find_library(force_feedback ForceFeedback REQUIRED)
    find_library(carbon Carbon REQUIRED)
    find_library(core_audio CoreAudio REQUIRED)
    find_library(audio_toolbox AudioToolbox REQUIRED)
    find_library(coremedia_lib CoreMedia REQUIRED)

    set(
            SDL2_PLATFORM_DEPS
            ${corevideo_lib}
            ${cocoa_lib}
            ${iokit_lib}
            ${force_feedback}
            ${carbon}
            ${core_audio}
            ${audio_toolbox}
            ${coremedia_lib}
            ${corefoundation_lib}
            ${avfoundation_lib}
            ${coregraphics_lib}
            iconv
    )
    add_definitions("-DSCREEN_HUNTER_MACOS")
endif()


file(GLOB_RECURSE APP_SRC src/app/*)
add_executable(ScreenShot ${APP_SRC})
target_include_directories(ScreenShot PRIVATE 3rdparty/include src/app)
target_link_libraries(ScreenShot blend2d screen_capture_lite SDL2 ${SDL2_PLATFORM_DEPS})


add_executable(TestScreenShotMac src/test/screenshot_mac.cpp)
target_include_directories(TestScreenShotMac PRIVATE 3rdparty/include)
target_link_libraries(TestScreenShotMac blend2d ${SDL2_PLATFORM_DEPS})