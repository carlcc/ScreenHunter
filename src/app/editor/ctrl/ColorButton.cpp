#include "editor/ctrl/ColorButton.h"
#include "window/Painter.h"

void ColorButton::paint(Painter& painter)
{
    if (visible()) {
        painter.setFillStyle(mFillColor);
        painter.setStrokeStyle(mSelectedBorderColor);

        painter.fillRoundRect(absX(), absY(), width(), height(), 4);
        if (mIsSelected) {
            painter.setStrokeWidth(2);
            painter.strokeRoundRect(absX(), absY(), width(), height(), 4);
        }

        if (mIsHover) {
            painter.setStrokeWidth(4);
            painter.strokeRoundRect(absX(), absY(), width(), height(), 4);
        }
    }
}
