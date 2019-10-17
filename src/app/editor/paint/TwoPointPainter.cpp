#include "TwoPointPainter.h"

void TwoPointPainter::onStart(int x, int y)
{
    mStartX = x;
    mStartY = y;
    mEndX = x;
    mEndY = y;
}

void TwoPointPainter::onEnd(int x, int y)
{
    mEndX = x;
    mEndY = y;
}

void TwoPointPainter::onDragging(int x, int y)
{
    mEndX = x;
    mEndY = y;
}
