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
    p.strokePath(mPath);
}
