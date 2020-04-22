#pragma once
#include <memory>
#include <set>

class AppWindow;

class AppWindowManager {
public:
    static AppWindowManager& get();

    template <class WindowClass>
    std::shared_ptr<WindowClass> createWindow(const std::string& title, int x, int y, int w, int h)
    {
        auto* pw = new WindowClass;
        auto win = registerWindow(pw, title, x, y, w, h);
        if (win == nullptr) {
            delete pw;
            return nullptr;
        }

        return std::static_pointer_cast<WindowClass>(win);
    }

    std::shared_ptr<AppWindow> registerWindow(AppWindow* win, const std::string& title, int x, int y, int w, int h);

    const std::set<AppWindow*>& allWindows() const;

private:
    AppWindowManager();
    ~AppWindowManager();

    class Impl;
    Impl* mImpl;
};
