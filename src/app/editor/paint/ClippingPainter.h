#pragma once

#include "editor/paint/TwoPointPainter.h"
#include <memory>

class BLImage;

class ClippingPainter : public TwoPointPainter {
public:
    ClippingPainter(const std::shared_ptr<BLImage>& img);

    void paint(Painter& p) override;

private:
    std::shared_ptr<BLImage> mImage;
};
