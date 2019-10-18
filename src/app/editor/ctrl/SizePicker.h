#pragma once

#include "editor/ctrl/ColorButton.h"
#include "editor/ctrl/Container.h"

class SizePicker : public Container {
public:
    SizePicker();
    ~SizePicker();

    int selectedSize() const;

protected:
    void onMouseButtonEvent(const MouseButtonEvent& mbe) override;
    void onKeyboardEvent(const KeyboardEvent& ke) override;
    void onMouseMoveEvent(const MouseMoveEvent& mme) override;
    void paint(Painter& painter) override;

    int mSelectedButton { 1 };
};