#pragma once

#include "editor/paint/TwoPointPainter.h"

class TextPainter : public TwoPointPainter {
public:
    void onText(const std::string& text) override;
    void paint(Painter& p) override;
    void endProcess() override;
    void onKey(bool isDown, ScanCode scanCode) override;

private:
    bool mDrawRect { true };
    std::string mText;
    float mFontSize { 20.F };
};
