//
// Created by carlcc on 2019/10/16.
//

#include "editor/EditorWindow.h"
#include "editor/ctrl/ColorPicker.h"
#include "editor/ctrl/SizePicker.h"
#include "editor/ctrl/TextBox.h"
#include "editor/ctrl/ToolPicker.h"
#include "editor/paint/ClippingPainter.h"
#include "editor/paint/IElementPainter.h"
#include "event/KeyboardEvent.h"
#include "event/MouseButtonEvent.h"
#include "event/MouseMoveEvent.h"
#include "window/Painter.h"
#include <chrono>
#include <event/TextInputEvent.h>
#include <iostream>

inline int64_t steadyTimeMillis()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

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
}

EditorWindow::~EditorWindow() = default;

void EditorWindow::paint()
{
    Painter painter(this);
    painter.clearAll();

    if (mImageToEdit != nullptr) {
        painter.blitImage(BLRectI(0, 0, width(), height()), *mImageToEdit, BLRectI(0, 0, mImageToEdit->width(), mImageToEdit->height()));
        painter.setFillStyle(BLRgba32(0xA0000000));
        painter.fillRect(BLRectI(0, 0, width(), height()));
    }

    BLPattern pattern(*mImageToEdit);
    pattern.scale(1.0 * width() / mImageToEdit->width(), 1.0 * height() / mImageToEdit->height());
    painter.setFillStyle(pattern);
    painter.setStrokeStyle(BLRgba32(0xFFFF0000));

    mPaintHistory.paint(painter);

    if (mPaintControls && mEditorState != ES_CLIPPING_IDLE && mEditorState != ES_CLIPPING) {
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
    if (mbe.button == MouseButtonEvent::BButtonLeft) {
        if (mEditorState == ES_CLIPPING_IDLE) {
            // Select painting area
            if (MouseButtonEvent::ADown == mbe.action) {
                mClippingPainter = std::make_shared<ClippingPainter>(mImageToEdit);
                setNewPaintTool(mClippingPainter);
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
            auto now = steadyTimeMillis();
            if (now - mLastTimeDownLeft < 150) {
                // done, copy to clipboard
                setClipBoard();
                return;
            } else {
                mLastTimeDownLeft = now;
            }

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
                setNewPaintTool(mToolPicker->newPaintTool());
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
    } else if (mbe.button == MouseButtonEvent::BButtonRight) {
        if (mbe.action == MouseButtonEvent::AUp) {
            if (mEditorState == ES_CLIPPING_IDLE) {
                // TODO gracefully exit
                exit(0);
            } else {
                mEditorState = ES_CLIPPING_IDLE;
                mPaintHistory.clear();
            }
        }
    }

    repaint();
}

void EditorWindow::onKeyboardEvent(const KeyboardEvent& ke)
{
    mCurrentPainter->onKey(ke.action == KeyboardEvent::ADown, ke.scanCode);
    //    mPaintControls = false;
    //    paint();
    //    BLImageCodec codec;
    //    codec.findByName("BMP");
    //    windowBuffer().writeToFile("a.bmp", codec);
    //    mPaintControls = true;
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
    mCurrentPainter->onText(tie.text);
    repaint();
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
void EditorWindow::setNewPaintTool(const std::shared_ptr<IElementPainter>& newTool)
{
    if (mCurrentPainter != nullptr) {
        mCurrentPainter->endProcess();
    }
    mCurrentPainter = newTool;
}
#include <clip.h>
void EditorWindow::setClipBoard()
{
    mPaintControls = false;
    mClippingPainter->setStrokeBorder(false);
    paint();
    mClippingPainter->setStrokeBorder(true);
    mPaintControls = true;

    BLImage img(mClippingPainter->width(), mClippingPainter->height(), BL_FORMAT_XRGB32);
    BLContext ctx(img);
    BLRectI srcArea(BLRectI(mClippingPainter->x(), mClippingPainter->y(), mClippingPainter->width(), mClippingPainter->height()));
    ctx.blitImage(BLPointI(0, 0), windowBuffer(), srcArea);

    BLImageData imgData {};
    img.getData(&imgData);
    clip::image_spec spec;
    spec.width = img.width();
    spec.height = img.height();
    spec.bytes_per_row = imgData.stride;
    spec.alpha_mask = 0xFF000000;
    spec.alpha_shift = 24;
    spec.red_mask = 0x00FF0000;
    spec.red_shift = 16;
    spec.green_mask = 0x0000FF00;
    spec.green_shift = 8;
    spec.blue_mask = 0x000000FF;
    spec.blue_shift = 0;
    spec.bits_per_pixel = 32;
    // seems to be only support ABGR32 ?
    clip::image clipImage(imgData.pixelData, spec);
    if (clip::set_image(clipImage)) {
        std::cout << "Suc" << std::endl;
    } else {
        std::cout << "Fail" << std::endl;
    }
}
