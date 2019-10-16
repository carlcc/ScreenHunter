#include "editor/paint/LinePainter.h"
#include "window/Painter.h"

void LinePainter::onStart(int x, int y)
{
    mStartX = x;
    mStartY = y;
}

void LinePainter::onEnd(int x, int y)
{
    mEndX = x;
    mEndY = y;
}

void LinePainter::onDragging(int x, int y)
{
    mEndX = x;
    mEndY = y;
}

void LinePainter::paint(Painter& p)
{
    p.strokeLine(mStartX, mStartY, mEndX, mEndY);
}
