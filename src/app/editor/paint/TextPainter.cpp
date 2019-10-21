#include "TextPainter.h"
#include "editor/paint/wqydkwmh.ttf.gen.h"
#include "window/Painter.h"
#include <iostream>

class TextPainterFontManager {
public:
    BLFont getFont(float size)
    {
        BLFont font;
        font.createFromFace(mFace, size);
        return font;
    }

    static TextPainterFontManager& get()
    {
        static TextPainterFontManager m;
        return m;
    }

private:
    TextPainterFontManager()
    {
        BLFontLoader loader;
        BLResult err = loader.createFromData(kFontData, sizeof(kFontData));
        if (err) {
            printf("Failed to load a font-face (err=%u)\n", err);
            return;
        }
        err = mFace.createFromLoader(loader, 0);
        if (err) {
            printf("Failed to load a font-face (err=%u)\n", err);
            return;
        }
    }

    BLFontFace mFace;
};

void TextPainter::onText(const std::string& text)
{
    std::cout << "Text Edit11or: " << text << std::endl;
    mText += text;
}

void TextPainter::paint(Painter& painter)
{
    mPaintStyle.apply(painter);
    painter.setStrokeWidth(1);
    painter.setStrokeStyle(BLRgba32(0xFFFF0000));

    painter.clipToRect(mStartX, mStartY, mEndX - mStartX + 1, mEndY - mStartY + 1);
    if (mDrawRect) {
        painter.strokeRect(mStartX, mStartY, mEndX - mStartX + 1, mEndY - mStartY + 1);
    }

    if (!mText.empty()) {
        BLFont font = TextPainterFontManager::get().getFont(mFontSize);
        BLFontMetrics fm = font.metrics();
        BLTextMetrics tm;
        BLGlyphBuffer gb;

        BLPoint p(x(), y() + fm.ascent);
        const char* text = mText.c_str();

        for (;;) {
            const char* end = strchr(text, '\n');
            gb.setUtf8Text(text, end ? (size_t)(end - text) : SIZE_MAX);
            font.shape(gb);
            font.getTextMetrics(gb, tm);

            p.x = x();
            painter.fillGlyphRun(p, font, gb.glyphRun());
            p.y += fm.ascent + fm.descent + fm.lineGap;

            if (!end)
                break;
            text = end + 1;
        }
    }
    painter.restoreClipping();
}

void TextPainter::onKey(bool isDown, ScanCode scanCode)
{
    if (isDown) {
        if (scanCode == ScanCode::SCANCODE_RETURN || scanCode == ScanCode::SCANCODE_RETURN2) {
            mText += "\n";
        }
    }
}

void TextPainter::endProcess()
{
    mDrawRect = false;
}
