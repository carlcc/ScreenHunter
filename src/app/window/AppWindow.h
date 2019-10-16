#pragma once
#include <string>
#include <blend2d.h>

class WindowEvent;
class MouseButtonEvent;
class MouseMoveEvent;
class KeyboardEvent;

class AppWindow {
public:
    void setTitle(const char* title);

    const char* title() const;

    void resize(int w, int h);

    void repaint();

    BLImage& windowBuffer();

    const BLImage& windowBuffer() const;

protected:
    AppWindow();
    ~AppWindow();

    void onPaint();

    virtual void paint();

    virtual void onWindowEvent(const WindowEvent& we);

    virtual void onMouseButtonEvent(const MouseButtonEvent& mbe);

    virtual void onKeyboardEvent(const KeyboardEvent& ke);

    virtual void onMouseMoveEvent(const MouseMoveEvent& mme);

private:
    bool needRepaint() const;

    bool initialize(const char* title, int w, int h);

    class Impl;
    Impl* mImpl;

    friend class App;
    friend class AppWindowManager;
};
