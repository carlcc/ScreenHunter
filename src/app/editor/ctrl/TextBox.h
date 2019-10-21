#include "editor/ctrl/Control.h"
#include <blend2d.h>

class TextBox : public Control {
public:
    void setText(const std::string& text)
    {
        mText = text;
    }

    const std::string& getText() const
    {
        return mText;
    }

    void setFontSize(float size)
    {
        mFontSize = size;
    }

    float getFontSize() const
    {
        return mFontSize;
    }

    void setTextColor(const BLRgba32& color)
    {
        mTextColor = color;
    }

    const BLRgba32& getTextColor() const
    {
        return mTextColor;
    }

protected:
    void paint(Painter& painter) override;
    void onMouseButtonEvent(const MouseButtonEvent& mbe) override;
    void onKeyboardEvent(const KeyboardEvent& ke) override;
    void onMouseMoveEvent(const MouseMoveEvent& mme) override;

private:
    std::string mText;
    BLRgba32 mTextColor;
    float mFontSize;
};
