#pragma once

#include "editor/ctrl/Control.h"
#include "editor/ctrl/icon/Icon.h"
#include <blend2d.h>
#include <memory>

class ToolButton : public Control {
public:
    void setSelectedColor(const BLRgba32& c)
    {
        mSelectedColor = c;
    }

    const BLRgba32& selectedColor() const
    {
        return mSelectedColor;
    }

    void setHoverColor(const BLRgba32& c)
    {
        mHoverColor = c;
    }

    const BLRgba32& hoverColor() const
    {
        return mHoverColor;
    }

    void setDefaultColor(const BLRgba32& c)
    {
        mDefaultColor = c;
    }

    const BLRgba32& defaultColor() const
    {
        return mDefaultColor;
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

    std::shared_ptr<Icon> icon() const
    {
        return mIcon;
    }

    void setIcon(const std::shared_ptr<Icon>& icon)
    {
        mIcon = icon;
    }

protected:
    void paint(Painter& painter) override;

public:
    std::shared_ptr<Icon> mIcon { nullptr };
    BLRgba32 mDefaultColor;
    BLRgba32 mSelectedColor;
    BLRgba32 mHoverColor;
    bool mIsSelected { false };
    bool mIsHover { false };
};
