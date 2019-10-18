#pragma once
#include "editor/paint/IElementPainter.h"

class TwoPointPainter : public IElementPainter {
public:
    void onStart(int x, int y) override;
    void onEnd(int x, int y) override;
    void onDragging(int x, int y) override;

protected:
    int mStartX { 0 };
    int mStartY { 0 };
    int mEndX { 0 };
    int mEndY { 0 };
};