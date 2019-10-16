#include "window/ctrl/ColorButton.h"
#include "window/Painter.h"

void ColorButton::paint(Painter& painter)
{
    painter.setFillStyle(mFillColor);
    painter.setStrokeStyle(mBorderColor);

    painter.fillRect(x(), y(), width(), height());
    if (mIsSelected) {
        painter.strokeRect(x(), y(), width(), height());
    }
}
