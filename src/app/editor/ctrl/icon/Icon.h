#pragma once

class Painter;

class Icon {
public:
    virtual void paint(Painter& painter, double x, double y, double w, double h) = 0;
};

class RectIcon : public Icon {
public:
    void paint(Painter& painter, double x, double y, double w, double h) override;
};

class EllipseIcon : public Icon {
public:
    void paint(Painter& painter, double x, double y, double w, double h) override;
};

class PenIcon : public Icon {
public:
    void paint(Painter& painter, double x, double y, double w, double h) override;
};

class TextIcon : public Icon {
public:
    void paint(Painter& painter, double x, double y, double w, double h) override;
};

class FilledEllipseIcon : public Icon {
public:
    void paint(Painter& painter, double x, double y, double w, double h) override;
};