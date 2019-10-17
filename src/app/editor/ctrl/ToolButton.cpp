#include "editor/ctrl/ToolButton.h"
#include "window/Painter.h"

void ToolButton::paint(Painter& painter)
{
    if (mIcon == nullptr) {
        return;
    }
    if (mIsHover) {
        painter.setStrokeWidth(4);
        painter.setStrokeStyle(mHoverColor);
    } else {
        painter.setStrokeWidth(2);
        if (mIsSelected) {
            painter.setStrokeStyle(mSelectedColor);
        } else {
            painter.setStrokeStyle(mDefaultColor);
        }
    }
    mIcon->paint(painter, absX(), absY(), width(), height());
}
