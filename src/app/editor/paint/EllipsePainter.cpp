#include "EllipsePainter.h"
#include "window/Painter.h"
#include <cmath>

void EllipsePainter::paint(Painter& p)
{
    double cx = (mStartX + mEndX) / 2.0;
    double cy = (mStartY + mEndY) / 2.0;
    double rx = std::abs((mStartX - mEndX) / 2.0);
    double ry = std::abs((mStartY - mEndY) / 2.0);

    mPaintStyle.apply(p);
    p.strokeEllipse(cx, cy, rx, ry);
}
