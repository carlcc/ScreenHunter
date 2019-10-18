#include "editor/paint/CurvePainter.h"
#include "window/Painter.h"

void CurvePainter::onStart(int x, int y)
{
    mPath.moveTo(x, y);
}

void CurvePainter::onEnd(int x, int y)
{
    mPath.lineTo(x, y);
}

void CurvePainter::onDragging(int x, int y)
{
    mPath.lineTo(x, y);
}

void CurvePainter::paint(Painter& p)
{
    mPaintStyle.apply(p);
    double cx = mPath.vertexData()->x;
    double cy = mPath.vertexData()->y;
    double rx = mPaintStyle.strokeWidth / 2.0;
    double ry = mPaintStyle.strokeWidth / 2.0;
    p.strokePath(mPath);

    p.fillEllipse(cx, cy, rx, ry);
    BLPoint point {};
    mPath.getLastVertex(&point);
    cx = point.x;
    cy = point.y;
    p.fillEllipse(cx, cy, rx, ry);
}
