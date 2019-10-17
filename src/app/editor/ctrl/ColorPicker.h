#pragma once

#include "editor/ctrl/ColorButton.h"
#include "editor/ctrl/Container.h"

class ColorPicker : public Container {
public:
    ColorPicker();
    ~ColorPicker();

    const BLRgba32& selectedColor() const;

protected:
    void onMouseButtonEvent(const MouseButtonEvent& mbe) override;
    void onKeyboardEvent(const KeyboardEvent& ke) override;
    void onMouseMoveEvent(const MouseMoveEvent& mme) override;
    void paint(Painter& painter) override;

    ColorButton* mSelectedButton { nullptr };
};
