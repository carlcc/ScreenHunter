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
        painter.setFillStyle(mHoverColor);
    } else {
        painter.setStrokeWidth(2);
        if (mIsSelected) {
            painter.setStrokeStyle(mSelectedColor);
            painter.setFillStyle(mSelectedColor);
        } else {
            painter.setStrokeStyle(mDefaultColor);
            painter.setFillStyle(mDefaultColor);
        }
    }
    double iconW = mIconWidth;
    double iconH = mIconHeight;
    double iconX = absX() + width() / 2.0 - iconW / 2.0;
    double iconY = absY() + height() / 2.0 - iconH / 2.0;
    mIcon->paint(painter, iconX, iconY, iconW, iconH);
}
