#pragma once

#include "editor/ctrl/Container.h"
#include <memory>

class IElementPainter;

class ToolPicker : public Container {
public:
    using ElementPainterCreator = std::shared_ptr<IElementPainter> (*)();

    ToolPicker();
    ~ToolPicker();

    std::shared_ptr<IElementPainter> newPaintTool();

protected:
    void onMouseButtonEvent(const MouseButtonEvent& mbe) override;
    void onKeyboardEvent(const KeyboardEvent& ke) override;
    void onMouseMoveEvent(const MouseMoveEvent& mme) override;
    void paint(Painter& painter) override;

    int mSelectedButton { -1 };
    ElementPainterCreator mElementPainterCreator { nullptr };
};
