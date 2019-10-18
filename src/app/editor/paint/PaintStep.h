#pragma once

#include "editor/paint/PaintStyle.h"

class Painter;

class PaintStep {
public:
    virtual void paint(Painter& p) = 0;

    void setPaintStyle(const PaintStyle& style)
    {
        mPaintStyle = style;
    }

protected:
    PaintStyle mPaintStyle {};
};
