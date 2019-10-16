#pragma once

#include "window/AppWindow.h"
#include "window/ctrl/ColorButton.h"

class EditorWindow : public AppWindow {
public:
    EditorWindow();
    ~EditorWindow();

protected:
    void paint() override;
    void onWindowEvent(const WindowEvent& we) override;
    void onMouseButtonEvent(const MouseButtonEvent& mbe) override;
    void onKeyboardEvent(const KeyboardEvent& ke) override;
    void onMouseMoveEvent(const MouseMoveEvent& mme) override;

private:
    ColorButton mButton1;
    ColorButton mButton2;
    ColorButton mButton3;
    friend class App;
};