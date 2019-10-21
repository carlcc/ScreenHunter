#include "ClippingPainter.h"
#include <window/Painter.h>

ClippingPainter::ClippingPainter(const std::shared_ptr<BLImage>& img): mImage(img)
{
}

void ClippingPainter::paint(Painter& p)
{
    p.fillRect(mStartX, mStartY, mEndX - mStartX + 1, mEndY - mStartY + 1);
    p.clipToRect(mStartX, mStartY, mEndX - mStartX + 1, mEndY - mStartY + 1);
}
