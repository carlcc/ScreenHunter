#pragma once

#include "editor/ctrl/Control.h"
#include <blend2d.h>

class ColorButton : public Control {
public:
    void setFillColor(const BLRgba32& c)
    {
        mFillColor = c;
    }

    void setSelectedBorderColor(const BLRgba32& c)
    {
        mSelectedBorderColor = c;
    }

    const BLRgba32& fillColor() const
    {
        return mFillColor;
    }

    const BLRgba32& selectedBorderColor() const
    {
        return mSelectedBorderColor;
    }

    void setHoverBorderColor(const BLRgba32& c) {
        mHoverBorderColor = c;
    }

    const BLRgba32& hoverBorderColor() const {
        return mHoverBorderColor;
    }

    void setSelected(bool b)
    {
        mIsSelected = b;
    }

    bool selected() const
    {
        return mIsSelected;
    }

    void setIsHover(bool b)
    {
        mIsHover = b;
    }

    bool isHover() const
    {
        return mIsHover;
    }

protected:
    void paint(Painter& painter) override;

public:
    BLRgba32 mFillColor;
    BLRgba32 mSelectedBorderColor;
    BLRgba32 mHoverBorderColor;
    bool mIsSelected { false };
    bool mIsHover { false };
};
