#include "PaintStyle.h"
#include "window/Painter.h"

void PaintStyle::apply(Painter& painter)
{
    painter.setStrokeWidth(strokeWidth);
    painter.setStrokeStyle(strokeColor);
    painter.setFillStyle(fillColor);
}
