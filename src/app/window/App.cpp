#include "window/App.h"
#include "event/KeyboardEvent.h"
#include "event/MouseButtonEvent.h"
#include "event/MouseMoveEvent.h"
#include "event/TextInputEvent.h"
#include "event/WindowEvent.h"
#include "window/AppWindow.h"
#include "window/AppWindowManager.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <map>

static App* gAppInstance { nullptr };
void setAppInstance(App* app)
{
    if (gAppInstance != nullptr && app != nullptr) {
        std::cerr << "Only one app instance is allowd!" << std::endl;
        abort();
    }
    gAppInstance = app;
}

App::App(int argc, char** argv)
    : mArgc(argc)
    , mArgv(argv)
    , mIsRunning(true)
    , mInitialized(false)
{
    mInitialized = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0;
    setAppInstance(this);
}

App::~App()
{
    setAppInstance(nullptr);
}

App* App::get()
{
    return gAppInstance;
}

int App::exec()
{
    if (!mInitialized) {
        return 1;
    }
    while (mIsRunning) {

        for (auto* appWin : AppWindowManager::get().allWindows()) {
            if (appWin->needRepaint()) {
                appWin->onPaint();
            }
        }
        handleEvents();
    }

    SDL_Quit();
    return 0;
}

void App::handleEvents()
{
    SDL_Event event;
    int count = 0;
    while (SDL_PollEvent(&event)) {
        ++count;
        switch (event.type) {
        case SDL_QUIT:
            stop();
            break;
        case SDL_WINDOWEVENT: {
            auto& e = event.window;
            auto* w = SDL_GetWindowFromID(e.windowID);
            if (w == nullptr) {
                break;
            }
            auto* appWin = (AppWindow*)SDL_GetWindowData(w, "app_win");
            WindowEvent we {};
            we.data1 = e.data1;
            we.data2 = e.data2;
            appWin->onWindowEvent(we);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN: {
            auto& e = event.button;
            auto* w = SDL_GetWindowFromID(e.windowID);
            if (w == nullptr) {
                break;
            }
            auto* appWin = (AppWindow*)SDL_GetWindowData(w, "app_win");
            MouseButtonEvent mbe {};
            mbe.x = e.x;
            mbe.y = e.y;
            static std::map<int, MouseButtonEvent::Button> kButtonMap {
                { SDL_BUTTON_LEFT, MouseButtonEvent::BButtonLeft },
                { SDL_BUTTON_RIGHT, MouseButtonEvent::BButtonRight },
                { SDL_BUTTON_MIDDLE, MouseButtonEvent::BButtonMid },
                { SDL_BUTTON_X1, MouseButtonEvent::BButtonX1 },
                { SDL_BUTTON_X2, MouseButtonEvent::BButtonX2 }
            };
            mbe.button = kButtonMap[int(e.button)];
            mbe.action = e.type == SDL_MOUSEBUTTONDOWN ? MouseButtonEvent::ADown : MouseButtonEvent::AUp;
            appWin->onMouseButtonEvent(mbe);
            break;
        }
        case SDL_MOUSEMOTION: {
            auto& e = event.motion;
            auto* w = SDL_GetWindowFromID(e.windowID);
            if (w == nullptr) {
                break;
            }
            auto* appWin = (AppWindow*)SDL_GetWindowData(w, "app_win");
            MouseMoveEvent mme {};
            mme.x = e.x;
            mme.y = e.y;
            mme.dx = e.xrel;
            mme.dy = e.yrel;
            appWin->onMouseMoveEvent(mme);
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            auto& e = event.key;
            auto* w = SDL_GetWindowFromID(e.windowID);
            if (w == nullptr) {
                break;
            }
            auto* appWin = (AppWindow*)SDL_GetWindowData(w, "app_win");
            KeyboardEvent ke {};
            ke.action = e.type == SDL_KEYDOWN ? KeyboardEvent::ADown : KeyboardEvent::AUp;
            ke.scanCode = ScanCode(e.keysym.scancode);
            appWin->onKeyboardEvent(ke);
            break;
        }
            //        case SDL_TEXTEDITING: {
            //            auto& e = event.edit;
            //            std::cout << "Edit: " << e.text << std::endl;
            //            break;
            //        }
        case SDL_TEXTINPUT: {
            auto& e = event.text;
            auto* w = SDL_GetWindowFromID(e.windowID);
            if (w == nullptr) {
                break;
            }
            auto* appWin = (AppWindow*)SDL_GetWindowData(w, "app_win");
            TextInputEvent tie {};
            tie.text = e.text;
            appWin->onTextInputEvent(tie);
            break;
        }
        default:
            //                        std::cout << "Unhandled event " << event.type << std::endl;
            break;
            // TODO
        }
    }
    if (count == 0) {
        SDL_Delay(1);
    }
}
