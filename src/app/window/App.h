#pragma once

#include "window/AppWindow.h"

class App {
public:
    App(int argc, char** argv);
    ~App();

    int exec();

    void stop() { mIsRunning = false; }

    static App* get();

private:
    void handleEvents();

    int mArgc;
    char** mArgv;
    bool mIsRunning;
    bool mInitialized;
};
