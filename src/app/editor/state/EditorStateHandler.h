#pragma once

class WindowEvent;
class MouseButtonEvent;
class KeyboardEvent;
class MouseMoveEvent;
class TextInputEvent;

class EditorStateHandler {
public:
    virtual void onWindowEvent(const WindowEvent& we) {}
    virtual void onMouseButtonEvent(const MouseButtonEvent& mbe) {}
    virtual void onKeyboardEvent(const KeyboardEvent& ke) {}
    virtual void onMouseMoveEvent(const MouseMoveEvent& mme) {}
    virtual void onTextInputEvent(const TextInputEvent& tie) {}
};