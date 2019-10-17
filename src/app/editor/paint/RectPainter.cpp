#include "RectPainter.h"
#include "window/Painter.h"

void RectPainter::paint(Painter& p)
{
    p.strokeRect(mStartX, mStartY, mEndX - mStartX + 1, mEndY - mStartY + 1);
}
