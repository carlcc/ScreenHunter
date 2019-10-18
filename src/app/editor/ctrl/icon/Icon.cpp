#include "editor/ctrl/icon/Icon.h"
#include "window/Painter.h"

void RectIcon::paint(Painter& painter, double x, double y, double w, double h)
{
    painter.strokeRect(x, y, w, h);
}

void EllipseIcon::paint(Painter& painter, double x, double y, double w, double h)
{
    double dx = x;
    double dy = y;
    double dw = w;
    double dh = h;

    painter.strokeEllipse(dx + dw / 2, dy + dw / 2, dw / 2, dh / 2);
}

BLPath fillPath(double xys[][2], int num, double x, double y, double w, double h)
{
    BLPath path;
    path.moveTo(xys[0][0] * w + x, xys[0][1] * h + y);
    for (int i = 1; i < num; ++i) {
        path.lineTo(xys[i][0] * w + x, xys[i][1] * h + y);
    }
    path.close();
    return path;
}

void PenIcon::paint(Painter& painter, double x, double y, double w, double h)
{
    // TODO
    {
        double head[][2] = {
            { 0.0, 1.0 },
            { 0.05, 0.8 },
            { 0.2, 0.95 }
        };
        const int num = sizeof(head) / sizeof(head[0]);
        BLPath path = fillPath(head, num, x, y, w, h);

        painter.fillPath(path);
        painter.strokePath(path);
    }

    {
        double body[][2] = {
            { 0.05, 0.8 },
            { 0.85, 0.0 },
            { 1.0, 0.15 },
            { 0.2, 0.95 },
        };
        const int num = sizeof(body) / sizeof(body[0]);
        BLPath path = fillPath(body, num, x, y, w, h);
        painter.strokePath(path);
    }
}

void TextIcon::paint(Painter& painter, double x, double y, double w, double h)
{
    painter.strokeLine(x, y, x + w, y);
    painter.strokeLine(x + w / 2.0, y, x + w / 2.0, y + h);
}

void FilledEllipseIcon::paint(Painter& painter, double x, double y, double w, double h)
{
    double dx = x;
    double dy = y;
    double dw = w;
    double dh = h;
    painter.fillEllipse(dx + dw / 2, dy + dw / 2, dw / 2, dh / 2);
}
