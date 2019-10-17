#pragma once

#include "editor/paint/TwoPointPainter.h"

class RectPainter : public TwoPointPainter{
public:
    void paint(Painter& p) override;
};

