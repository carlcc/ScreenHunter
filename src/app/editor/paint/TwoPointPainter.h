#pragma once
#include "editor/paint/IElementPainter.h"

class TwoPointPainter : public IElementPainter {
public:
    void onStart(int x, int y) override;
    void onEnd(int x, int y) override;
    void onDragging(int x, int y) override;

    int x() const {
        return mStartX < mEndX ? mStartX : mEndX;
    }
    int y() const {
        return mStartY < mEndY ? mStartY : mEndY;
    }
    int width() const {
        return mStartX < mEndX ? mEndX - mStartX + 1 : mStartX - mEndX + 1;
    }

    int height() const {
        return mStartY < mEndY ? mEndY - mStartY + 1 : mStartY - mEndY + 1;
    }

protected:
    int mStartX { 0 };
    int mStartY { 0 };
    int mEndX { 0 };
    int mEndY { 0 };
};
