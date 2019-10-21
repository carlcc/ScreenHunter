cmake_minimum_required(VERSION 3.2)

project(ScreenHunter)

set(CMAKE_CXX_STANDARD 14)

set(BLEND2D_STATIC ON)
set(SDL_SHARED OFF)
add_subdirectory(3rdparty/SDL2-2.0.8)
add_subdirectory(3rdparty/blend2d)

file(GLOB EXAMPLE src/example/*)
file(GLOB EXAMPLE_PATH src/example)

foreach (proj ${EXAMPLE})
    file(GLOB_RECURSE proj_src ${proj}/*)
    string(REPLACE "${EXAMPLE_PATH}/" "" out_name ${proj})
    message(${out_name})
    add_executable(${out_name} ${proj_src} )
    target_include_directories(${out_name} PRIVATE 3rdparty/include)
    target_link_libraries(${out_name} blend2d)
endforeach (proj)

if (WIN32)
    set(
            PLATFORM_DEPS
            SDL2-static
            user32
            gdi32
            winmm
            imm32
            ole32
            oleaut32
            version
            uuid
            dinput8
    )

    add_definitions("-DSCREEN_HUNTER_WIN32")
    file(GLOB PLATFORM_SRC_FILES src/app/screen/win32/*)
elseif(APPLE)
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
            PLATFORM_DEPS
            SDL2-static
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
    file(GLOB PLATFORM_SRC_FILES src/app/screen/mac/*)
elseif(UNIX)
    set(
            PLATFORM_DEPS
            SDL2-static
            X11
            pthread
            dl
            rt
    )

    add_definitions("-DSCREEN_HUNTER_X11")
    file(GLOB PLATFORM_SRC_FILES src/app/screen/x11/*)
endif()


file(GLOB APP_SRC
        src/app/*
        src/app/editor/*
        src/app/editor/ctrl/*
        src/app/editor/ctrl/icon/*
        src/app/editor/paint/*
        src/app/event/*
        src/app/screen/*
        src/app/window/*
)

add_executable(ScreenShot ${APP_SRC} ${PLATFORM_SRC_FILES})
target_include_directories(ScreenShot PRIVATE 3rdparty/include src/app)
target_link_libraries(ScreenShot blend2d ${PLATFORM_DEPS})
add_dependencies(ScreenShot blend2d SDL2-static)

if (APPLE)
    add_executable(TestScreenShotMac src/test/screenshot_mac.cpp)
    target_include_directories(TestScreenShotMac PRIVATE 3rdparty/include)
    target_link_libraries(TestScreenShotMac blend2d ${PLATFORM_DEPS})
elseif(UNIX)
    add_executable(TestScreenShotLinux src/test/screenshot_linux.cpp)
    target_include_directories(TestScreenShotLinux PRIVATE 3rdparty/include)
    target_link_libraries(TestScreenShotLinux blend2d ${PLATFORM_DEPS})
elseif(WIN32)
    add_executable(TestScreenShotWindows src/test/screenshot_windows.cpp)
    target_include_directories(TestScreenShotWindows PRIVATE 3rdparty/include)
    target_link_libraries(TestScreenShotWindows blend2d ${PLATFORM_DEPS})
endif()