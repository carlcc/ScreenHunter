#pragma once

#include "editor/paint/IDragHandler.h"
#include "editor/paint/IInputTextHandler.h"
#include "editor/paint/PaintStep.h"

class IElementPainter : public PaintStep, public IDragHandler, public IInputTextHandler {
public:
    void onStart(int x, int y) override
    {
    }
    void onEnd(int x, int y) override
    {
    }
    void onDragging(int x, int y) override
    {
    }
    void onText(const std::string& text) override
    {
    }
    void paint(Painter& p) override
    {
    }
    void onKey(bool isDown, ScanCode scanCode) override
    {
    }

    virtual void endProcess() {}
};
