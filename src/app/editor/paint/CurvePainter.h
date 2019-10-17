#pragma once
#include "editor/paint/IElementPainter.h"
#include <blend2d.h>
#include <tuple>
#include <vector>

class CurvePainter : public IElementPainter {
public:
    void onStart(int x, int y) override;
    void onEnd(int x, int y) override;
    void onDragging(int x, int y) override;
    void paint(Painter& p) override;

private:
    BLPath mPath;
};
