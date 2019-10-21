#include "SizePicker.h"
#include "editor/ctrl/ToolButton.h"
#include "event/MouseButtonEvent.h"
#include "event/MouseMoveEvent.h"
#include "window/Painter.h"
#include <vector>

class SizePickerConst {
public:
    static const std::vector<int>& buttonSizes()
    {
        const static std::vector<int> buttonsSizes = {
            2, 4, 8, 16
        };
        return buttonsSizes;
    }
};

SizePicker::SizePicker()
{
    const int kButtonGap = 10;
    const int kButtonWidth = 32;
    const uint32_t kSelectedColor = 0xff338080;
    const uint32_t kHoverColor = 0xff338080;
    const uint32_t kDefaultColor = 0xfff0f0f0;
    const int kMarginLeft = 10;
    const int kMarginTop = 10;

    const auto& icon = std::make_shared<FilledEllipseIcon>();
    const auto& buttonSizes = SizePickerConst::buttonSizes();
    const int numButtons = int(buttonSizes.size());
    for (auto i = 0; i < numButtons; ++i) {
        std::shared_ptr<ToolButton> btn = std::make_shared<ToolButton>();

        btn->setSize(kButtonWidth, kButtonWidth);
        btn->setIconSize(buttonSizes[i], buttonSizes[i]);
        btn->setSelectedColor(BLRgba32(kSelectedColor));
        btn->setHoverColor(BLRgba32(kHoverColor));
        btn->setDefaultColor(BLRgba32(kDefaultColor));
        btn->setPosition(kMarginLeft + (kButtonWidth + kButtonGap) * i, kMarginTop);
        btn->setIcon(icon);

        auto ctl = std::static_pointer_cast<Control>(btn);
        addChild(ctl);
    }

    mSelectedButton = 0;
    ((ToolButton*)childAt(mSelectedButton).get())->setSelected(true);

    setSize((kButtonGap + kButtonWidth) * numButtons - kButtonGap + kMarginLeft * 2, (kButtonGap + kButtonWidth) - kButtonGap + kMarginTop * 2);
}

SizePicker::~SizePicker()
{
}

int SizePicker::selectedSize() const
{
    return SizePickerConst::buttonSizes()[mSelectedButton];
}

void SizePicker::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
    if (mbe.action == MouseButtonEvent::ADown) {
        int i = 0;
        for (auto& c : children()) {
            auto btn = std::static_pointer_cast<ToolButton>(c);

            auto isSelected = btn->isInnerAbs(mbe.x, mbe.y);
            if (isSelected) {
                mSelectedButton = i;
            }
            btn->setSelected(false);
            ++i;
        }
        ((ToolButton*)childAt(mSelectedButton).get())->setSelected(true);
    }
}

void SizePicker::onKeyboardEvent(const KeyboardEvent& ke)
{
    Container::onKeyboardEvent(ke);
}

void SizePicker::onMouseMoveEvent(const MouseMoveEvent& mme)
{
    for (auto& c : children()) {
        auto btn = std::static_pointer_cast<ToolButton>(c);

        auto isSelected = btn->isInnerAbs(mme.x, mme.y);
        btn->setIsHover(isSelected);
    }
}

void SizePicker::paint(Painter& painter)
{
    if (visible()) {
        painter.setFillStyle(BLRgba32(uint32_t(0xff1234557)));
        painter.fillRoundRect(absX(), absY(), width(), height(), 5);
        Container::paint(painter);
    }
}
