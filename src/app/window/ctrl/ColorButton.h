#pragma once

#include "window/ctrl/Control.h"
#include <blend2d.h>

class ColorButton : public Control {
public:
    void paint(Painter& painter) override;

    void setFillColor(const BLRgba32& c)
    {
        mFillColor = c;
    }

    void setBorderColor(const BLRgba32& c)
    {
        mBorderColor = c;
    }

    const BLRgba32& fillColor() const
    {
        return mFillColor;
    }

    const BLRgba32& borderColor() const
    {
        return mBorderColor;
    }

    void setSelected(bool b) {
        mIsSelected = b;
    }

    bool selected() const {
        return mIsSelected;
    }

public:
    BLRgba32 mFillColor;
    BLRgba32 mBorderColor;
    bool mIsSelected { false };
};
