#include "editor/ctrl/ColorPicker.h"
#include "editor/ctrl/ColorButton.h"
#include "event/MouseButtonEvent.h"
#include "event/MouseMoveEvent.h"
#include "window/Painter.h"

ColorPicker::ColorPicker()
{
    const uint32_t colors[] = {
        0xffff0000, // red
        0xff00ff00, // green
        0xff0000ff, // blue
        0xff000000, // black
        0xffffffff, // white
        0xffffff00, //
        0xffff00ff, //
        0xff00ffff,
    };
    const int kButtonGap = 4;
    const int kButtonWidth = 20;
    const uint32_t kSelectedBorderColor = 0xdddddddd;
    const uint32_t kHoverBorderColor = 0x7f7f7f7f;
    const int kMarginLeft = 4;
    const int kMarginTop = 4;

    for (auto i = 0; i < 8; ++i) {
        std::shared_ptr<ColorButton> btn = std::make_shared<ColorButton>();

        btn->setSize(kButtonWidth, kButtonWidth);
        btn->setFillColor(BLRgba32(colors[i]));
        btn->setSelectedBorderColor(BLRgba32(kSelectedBorderColor));
        btn->setHoverBorderColor(BLRgba32(kHoverBorderColor));
        btn->setPosition(kMarginLeft + (kButtonWidth + kButtonGap) * (i % 4), kMarginTop + (kButtonWidth + kButtonGap) * (i / 4));

        auto ctl = std::static_pointer_cast<Control>(btn);
        addChild(ctl);
    }

    mSelectedButton = (ColorButton*)childAt(0).get();
    mSelectedButton->setSelected(true);
    setSize((kButtonGap + kButtonWidth) * 4 - kButtonGap + kMarginLeft * 2, (kButtonGap + kButtonWidth) * 2 - kButtonGap + kMarginTop * 2);
}

ColorPicker::~ColorPicker() = default;

void ColorPicker::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
    if (mbe.action == MouseButtonEvent::ADown) {
        for (auto& c : children()) {
            auto btn = std::static_pointer_cast<ColorButton>(c);

            auto isSelected = btn->isInnerAbs(mbe.x, mbe.y);
            if (isSelected) {
                mSelectedButton = btn.get();
            }
            btn->setSelected(false);
        }
        mSelectedButton->setSelected(true);
    }
}

void ColorPicker::onKeyboardEvent(const KeyboardEvent& ke)
{
    Container::onKeyboardEvent(ke);
}

void ColorPicker::onMouseMoveEvent(const MouseMoveEvent& mme)
{
    for (auto& c : children()) {
        auto btn = std::static_pointer_cast<ColorButton>(c);

        auto isSelected = btn->isInnerAbs(mme.x, mme.y);
        btn->setIsHover(isSelected);
    }
}

void ColorPicker::paint(Painter& painter)
{
    if (visible()) {
        painter.setFillStyle(BLRgba32(uint32_t(0xff1234557)));
        painter.fillRoundRect(absX(), absY(), width(), height(), 5);
        Container::paint(painter);
    }
}

const BLRgba32& ColorPicker::selectedColor() const
{
    return mSelectedButton->fillColor();
}
