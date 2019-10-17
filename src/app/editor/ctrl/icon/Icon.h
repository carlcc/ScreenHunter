#pragma once

class Painter;

class Icon {
public:
    virtual void paint(Painter& painter, int x, int y, int w, int h) = 0;
};

class RectIcon : public Icon {
public:
    void paint(Painter& painter, int x, int y, int w, int h) override;
};

class EllipseIcon : public Icon {
public:
    void paint(Painter& painter, int x, int y, int w, int h) override;
};

class PenIcon : public Icon {
public:
    void paint(Painter& painter, int x, int y, int w, int h) override;
};

class TextIcon : public Icon {
public:
    void paint(Painter& painter, int x, int y, int w, int h) override;
};