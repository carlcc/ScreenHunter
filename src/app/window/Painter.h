#pragma once

#include <blend2d.h>

class AppWindow;

class Painter : public BLContext {
public:
    explicit Painter(AppWindow* w);
    ~Painter();
};
