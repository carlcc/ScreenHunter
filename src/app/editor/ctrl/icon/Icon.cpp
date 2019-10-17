#include "editor/ctrl/icon/Icon.h"
#include "window/Painter.h"

void RectIcon::paint(Painter& painter, int x, int y, int w, int h)
{
    painter.strokeRect(x, y, w, h);
}

void EllipseIcon::paint(Painter& painter, int x, int y, int w, int h)
{
    double dx = x;
    double dy = y;
    double dw = w;
    double dh = h;

    painter.strokeEllipse(dx + dw / 2, dy + dw / 2, dw / 2, dh / 2);
}

void PenIcon::paint(Painter& painter, int x, int y, int w, int h)
{
    // TODO
    BLPath path;
    path.moveTo(0, 0);
    //    path.lineTo(0.2, 0.1);
    //    path.lineTo()
    painter.strokePath(path);
}

void TextIcon::paint(Painter& painter, int x, int y, int w, int h)
{
    painter.strokeLine(x, y, x + w, y);
    painter.strokeLine(x + w / 2.0, y, x + w / 2.0, y + h);
}
