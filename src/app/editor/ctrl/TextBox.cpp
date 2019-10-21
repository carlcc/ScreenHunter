#include "editor/ctrl/TextBox.h"
#include "window/Painter.h"

class TextBoxFontManager {
public:
    BLFont getFont(float size)
    {
        BLFont font;
        font.createFromFace(mFace, size);
        return font;
    }

    static TextBoxFontManager& get()
    {
        static TextBoxFontManager m;
        return m;
    }

private:
    TextBoxFontManager()
    {
        // TODO bundle to program, and load from memory program space
        BLResult err = mFace.createFromFile("NotoSans-Regular.ttf");
        if (err) {
            printf("Failed to load a font-face (err=%u)\n", err);
            return;
        }
    }

    BLFontFace mFace;
};

void TextBox::paint(Painter& painter)
{
    painter.strokeRect(absX(), absY(), width(), height());
    painter.setStrokeStyle(mTextColor);

    BLFont font = TextBoxFontManager::get().getFont(mFontSize);
    BLFontMetrics fm = font.metrics();
    BLTextMetrics tm;
    BLGlyphBuffer gb;

    BLPoint p(absX(), absY() + fm.ascent);
    const char* text = mText.c_str();

    for (;;) {
        const char* end = strchr(text, '\n');
        gb.setUtf8Text(text, end ? (size_t)(end - text) : SIZE_MAX);
        font.shape(gb);
        font.getTextMetrics(gb, tm);

        p.x = absX();
        painter.fillGlyphRun(p, font, gb.glyphRun());
        p.y += fm.ascent + fm.descent + fm.lineGap;

        if (!end)
            break;
        text = end + 1;
    }
}

void TextBox::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
    Control::onMouseButtonEvent(mbe);
}

void TextBox::onKeyboardEvent(const KeyboardEvent& ke)
{
    Control::onKeyboardEvent(ke);
}

void TextBox::onMouseMoveEvent(const MouseMoveEvent& mme)
{
    Control::onMouseMoveEvent(mme);
}
