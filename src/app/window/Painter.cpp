//
// Created by carlcc on 2019/10/16.
//

#include "window/Painter.h"
#include "window/AppWindow.h"
#include "window/AppWindowImpl.h"

Painter::Painter(AppWindow* w)
    : BLContext(w->windowBuffer())
{
}

Painter::~Painter() = default;
