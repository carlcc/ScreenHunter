#pragma once

#include "editor/paint/TwoPointPainter.h"
#include <memory>

class BLImage;

class ClippingPainter : public TwoPointPainter {
public:
    ClippingPainter(const std::shared_ptr<BLImage>& img);

    void paint(Painter& p) override;

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


private:
    std::shared_ptr<BLImage> mImage;
};
