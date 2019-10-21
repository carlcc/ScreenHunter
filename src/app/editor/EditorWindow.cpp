//
// Created by carlcc on 2019/10/16.
//

#include "editor/EditorWindow.h"
#include "editor/ctrl/ColorPicker.h"
#include "editor/ctrl/SizePicker.h"
#include "editor/ctrl/ToolPicker.h"
#include "editor/paint/ClippingPainter.h"
#include "editor/paint/IElementPainter.h"
#include "event/KeyboardEvent.h"
#include "event/MouseButtonEvent.h"
#include "event/MouseMoveEvent.h"
#include "window/Painter.h"
#include <iostream>

EditorWindow::EditorWindow()
{
    int i = 0;
    mControls.push_back(std::make_shared<ColorPicker>());
    mControls.push_back(std::make_shared<ToolPicker>());
    mControls.push_back(std::make_shared<SizePicker>());

    mColorPicker = static_cast<ColorPicker*>(mControls[i++].get()); // NOLINT
    mToolPicker = static_cast<ToolPicker*>(mControls[i++].get()); // NOLINT
    mSizePicker = static_cast<SizePicker*>(mControls[i++].get()); // NOLINT

    mColorPicker->setPosition(100, 100);
    mToolPicker->setPosition(300, 100);
    mSizePicker->setPosition(100, 300);
    mCurrentPainter = mToolPicker->newPaintTool();
}

EditorWindow::~EditorWindow() = default;

void EditorWindow::paint()
{
    Painter painter(this);
    painter.clearAll();

    if (mImageToEdit != nullptr) {
        painter.blitImage(BLRectI(0, 0, width(), height()), *mImageToEdit, BLRectI(0, 0, mImageToEdit->width(), mImageToEdit->height()));
        painter.setFillStyle(BLRgba32(0, 0, 0, 144));
        painter.fillRect(BLRectI(0, 0, width(), height()));
    }

    BLPattern pattern(*mImageToEdit);
    pattern.scale(1.0 * width() / mImageToEdit->width(), 1.0 * height() / mImageToEdit->height());
    painter.setFillStyle(pattern);

    mPaintHistory.paint(painter);

    if (mPaintControls && mEditorState == ES_PAINTING_IDLE) {
        painter.restoreClipping();
        for (auto& c : mControls) {
            c->paint(painter);
        }
    }
}

void EditorWindow::onWindowEvent(const WindowEvent& we)
{
    repaint();
}

void EditorWindow::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
    if (mEditorState == ES_CLIPPING_IDLE) {
        // Select painting area
        if (MouseButtonEvent::ADown == mbe.action) {
            mClippingPainter = std::make_shared<ClippingPainter>(mImageToEdit);
            mCurrentPainter = mClippingPainter;
            mCurrentPainter->onStart(mbe.x, mbe.y);
            mPaintHistory.push(mCurrentPainter);
            mEditorState = ES_CLIPPING;
        }
    } else if (mEditorState == ES_CLIPPING) {
        if (MouseButtonEvent::AUp == mbe.action) {
            mCurrentPainter->onEnd(mbe.x, mbe.y);
            setButtonsPosition();
            mEditorState = ES_PAINTING_IDLE;
        }
    } else if (mEditorState == ES_PAINTING_IDLE) {
        for (auto rit = mControls.rbegin(); rit != mControls.rend(); ++rit) {
            auto pControl = *rit;
            if (pControl->isInnerAbs(mbe.x, mbe.y)) {
                pControl->onMouseButtonEvent(mbe);
                repaint();
                return;
            }
        }
        if (MouseButtonEvent::ADown == mbe.action) {
            mEditorState = ES_PAINTING;
            mCurrentPainter = mToolPicker->newPaintTool();
            PaintStyle ps;
            ps.strokeColor = mColorPicker->selectedColor();
            ps.strokeWidth = mSizePicker->selectedSize();
            ps.fillColor = mColorPicker->selectedColor();

            mCurrentPainter->setPaintStyle(ps);
            mCurrentPainter->onStart(mbe.x, mbe.y);
            mPaintHistory.push(mCurrentPainter);
            std::cout << "paint history size: " << mPaintHistory.totalSize() << ", " << mPaintHistory.effectiveSize() << std::endl;
        }
    } else if (mEditorState == ES_PAINTING) {
        if (MouseButtonEvent::AUp == mbe.action) {
            mCurrentPainter->onEnd(mbe.x, mbe.y);
            mEditorState = ES_PAINTING_IDLE;
        }
    }

    repaint();
}

void EditorWindow::onKeyboardEvent(const KeyboardEvent& ke)
{
    AppWindow::onKeyboardEvent(ke);
    mPaintControls = false;
    paint();
    BLImageCodec codec;
    codec.findByName("BMP");
    windowBuffer().writeToFile("a.bmp", codec);
    mPaintControls = true;
}

void EditorWindow::onMouseMoveEvent(const MouseMoveEvent& mme)
{
    if (mEditorState == ES_CLIPPING) {
        mCurrentPainter->onDragging(mme.x, mme.y);
        repaint();
        return;
    }
    if (mEditorState == ES_PAINTING) {
        mCurrentPainter->onDragging(mme.x, mme.y);
        repaint();
        return;
    }

    if (mEditorState == ES_PAINTING_IDLE) {
        for (auto rit = mControls.rbegin(); rit != mControls.rend(); ++rit) {
            auto pControl = *rit;
            if (pControl->isInnerAbs(mme.x, mme.y)) {
                pControl->onMouseMoveEvent(mme);
                repaint();
                return;
            }
        }
    }
    repaint();
}

void EditorWindow::onTextInputEvent(const TextInputEvent& tie)
{
    AppWindow::onTextInputEvent(tie);
}

void EditorWindow::setImage(const std::shared_ptr<BLImage>& img)
{
    mImageToEdit = img;
}

void EditorWindow::setButtonsPosition()
{
    const int kGap = 8;
    int barHeight = mColorPicker->height();
    int barWidth = mColorPicker->width() + mSizePicker->width() + mToolPicker->width() + kGap * 2;
    int x = mClippingPainter->x();
    int y = mClippingPainter->y();
    int h = mClippingPainter->height();

    if (x + barWidth > width()) {
        x = width() - barWidth;
    }
    if (y + h + barHeight + kGap > height()) {
        // try to figure out if we can put the bar on the top of the clipping area
        if (y - kGap - barHeight < 0) {
            // no
            y = height() - barHeight;
        } else {
            // yes
            y = y - kGap - barHeight;
        }
    } else {
        y = y + h + kGap;
    }

    mColorPicker->setPosition(x, y);
    mSizePicker->setPosition(x + mColorPicker->width() + kGap, y);
    mToolPicker->setPosition(x + mColorPicker->width() + mSizePicker->width() + kGap * 2, y);
}
