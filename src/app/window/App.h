#pragma once

#include "window/AppWindow.h"

class App {
public:
    App(int argc, char** argv);

    int exec();

private:
    void handleEvents();

    int mArgc;
    char** mArgv;
    bool mIsRunning;
    bool mInitialized;
};
