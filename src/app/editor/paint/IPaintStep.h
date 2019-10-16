#pragma once

class Painter;

class IPaintStep {
public:
    virtual void paint(Painter& p) = 0;
};
