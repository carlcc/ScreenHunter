#include "editor/ctrl/ToolPicker.h"
#include "editor/ctrl/ToolButton.h"
#include "editor/paint/CurvePainter.h"
#include "editor/paint/EllipsePainter.h"
#include "editor/paint/RectPainter.h"
#include "event/MouseButtonEvent.h"
#include "event/MouseMoveEvent.h"
#include "window/Painter.h"

class ToolPickerConst {
public:
    static const std::vector<ToolPicker::ElementPainterCreator>& elementPainterCreators()
    {
        static std::vector<ToolPicker::ElementPainterCreator> kCreators = {
            [] { return std::static_pointer_cast<IElementPainter>(std::make_shared<RectPainter>()); },
            [] { return std::static_pointer_cast<IElementPainter>(std::make_shared<EllipsePainter>()); },
            [] { return std::static_pointer_cast<IElementPainter>(std::make_shared<CurvePainter>()); },
            [] { return std::static_pointer_cast<IElementPainter>(std::make_shared<CurvePainter>()); },
        };
        return kCreators;
    }

    static const std::vector<std::shared_ptr<Icon>>& icons()
    {
        static std::vector<std::shared_ptr<Icon>> kIcons = {
            std::make_shared<RectIcon>(),
            std::make_shared<EllipseIcon>(),
            std::make_shared<PenIcon>(),
            std::make_shared<TextIcon>()
        };
        return kIcons;
    }

    static int num()
    {
        return int(elementPainterCreators().size());
    }
};

ToolPicker::ToolPicker()
{
    const int kButtonGap = 10;
    const int kButtonWidth = 30;
    const uint32_t kSelectedColor = 0xff338080;
    const uint32_t kHoverColor = 0xff338080;
    const uint32_t kDefaultColor = 0xfff0f0f0;
    const int kMarginLeft = 8;
    const int kMarginTop = 8;

    const auto& icons = ToolPickerConst::icons();
    int numButtons = int(icons.size());
    for (auto i = 0; i < numButtons; ++i) {
        std::shared_ptr<ToolButton> btn = std::make_shared<ToolButton>();

        btn->setSize(kButtonWidth, kButtonWidth);
        btn->setSelectedColor(BLRgba32(kSelectedColor));
        btn->setHoverColor(BLRgba32(kHoverColor));
        btn->setDefaultColor(BLRgba32(kDefaultColor));
        btn->setPosition(kMarginLeft + (kButtonWidth + kButtonGap) * i, kMarginTop);
        btn->setIcon(icons[i]);

        auto ctl = std::static_pointer_cast<Control>(btn);
        addChild(ctl);
    }

    mSelectedButton = 0;
    ((ToolButton*)childAt(mSelectedButton).get())->setSelected(true);

    setSize((kButtonGap + kButtonWidth) * numButtons - kButtonGap + kMarginLeft * 2, (kButtonGap + kButtonWidth) - kButtonGap + kMarginTop * 2);
}

ToolPicker::~ToolPicker()
{
}

void ToolPicker::onMouseButtonEvent(const MouseButtonEvent& mbe)
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

void ToolPicker::onKeyboardEvent(const KeyboardEvent& ke)
{
    Container::onKeyboardEvent(ke);
}

void ToolPicker::onMouseMoveEvent(const MouseMoveEvent& mme)
{
    for (auto& c : children()) {
        auto btn = std::static_pointer_cast<ToolButton>(c);

        auto isSelected = btn->isInnerAbs(mme.x, mme.y);
        btn->setIsHover(isSelected);
    }
}

void ToolPicker::paint(Painter& painter)
{
    if (visible()) {
        painter.setFillStyle(BLRgba32(uint32_t(0xff1234557)));
        painter.fillRoundRect(absX(), absY(), width(), height(), 5);
        Container::paint(painter);
    }
}
std::shared_ptr<IElementPainter> ToolPicker::newPaintTool()
{
    return ToolPickerConst::elementPainterCreators()[mSelectedButton]();
}
