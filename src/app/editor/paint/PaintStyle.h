#pragma once
#include <blend2d.h>
#include <cstdint>

class Painter;
struct PaintStyle {
    int strokeWidth { 1 };
    BLRgba32 fillColor { 0xFFFF0000 };
    BLRgba32 strokeColor { 0xFFFF0000 };

    void apply(Painter& painter);
};