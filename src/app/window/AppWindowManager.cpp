//
// Created by carlcc on 2019/10/16.
//

#include "window/AppWindowManager.h"
#include "window/AppWindow.h"
#include <set>

class AppWindowManager::Impl {
public:
    std::set<AppWindow*> mWindows;
};

AppWindowManager& AppWindowManager::get()
{
    static AppWindowManager i;
    return i;
}

AppWindowManager::AppWindowManager()
    : mImpl(new AppWindowManager::Impl)
{
}

AppWindowManager::~AppWindowManager()
{
    delete mImpl;
}
const std::set<AppWindow*>& AppWindowManager::allWindows() const
{
    return mImpl->mWindows;
}

std::shared_ptr<AppWindow> AppWindowManager::registerWindow(AppWindow* win, const std::string& title, int w, int h)
{
    if (!win->initialize(title.c_str(), w, h)) {
        return nullptr;
    }

    auto swin = std::shared_ptr<AppWindow>(win, [this](auto* pw) {
        mImpl->mWindows.erase(pw);
        delete pw;
    });
    mImpl->mWindows.insert(win);
    return swin;
}
