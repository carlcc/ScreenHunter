#pragma once
#include "editor/paint/TwoPointPainter.h"

class EllipsePainter : public TwoPointPainter {
public:
    void paint(Painter& p) override;
};
