#pragma once
#include <blend2d.h>
#include <string>

class WindowEvent;
class MouseButtonEvent;
class MouseMoveEvent;
class KeyboardEvent;
class TextInputEvent;

class AppWindow {
public:
    void setTitle(const char* title);

    const char* title() const;

    void resize(int w, int h);

    int width() const;

    int height() const;

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

    virtual void onTextInputEvent(const TextInputEvent& tie);

private:
    bool needRepaint() const;

    bool initialize(const char* title, int x, int y, int w, int h);

    class Impl;
    Impl* mImpl;

    friend class App;
    friend class AppWindowManager;
};
