//
// Created by carlcc on 2019/10/16.
//

#include "editor/EditorWindow.h"
#include "editor/ctrl/ColorButton.h"
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

    mColorButtons = static_cast<ColorPicker*>(mControls[i++].get()); // NOLINT
    mToolButtons = static_cast<ToolPicker*>(mControls[i++].get()); // NOLINT

    mColorButtons->setPosition(100, 100);
    mToolButtons->setPosition(300, 100);
    mCurrentPainter = mToolButtons->newPaintTool();
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::paint()
{
    AppWindow::paint();

    Painter painter(this);
    painter.clearAll();
    painter.setFillStyle(BLRgba32(0xff00ffff));
    painter.setStrokeStyle(BLRgba32(mColorButtons->selectedColor()));
    painter.setStrokeWidth(4);

    mPaintHistory.paint(painter);

    for (auto& c : mControls) {
        c->paint(painter);
    }
}

void EditorWindow::onWindowEvent(const WindowEvent& we)
{
    AppWindow::onWindowEvent(we);
}

void EditorWindow::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
    if (!mIsPainting) {
        for (auto rit = mControls.rbegin(); rit != mControls.rend(); ++rit) {
            auto pControl = *rit;
            if (pControl->isInnerAbs(mbe.x, mbe.y)) {
                pControl->onMouseButtonEvent(mbe);
                repaint();
                return;
            }
        }
    }

    mIsPainting = MouseButtonEvent::ADown == mbe.action;
    if (mIsPainting) {
        mCurrentPainter = mToolButtons->newPaintTool();
        mCurrentPainter->onStart(mbe.x, mbe.y);
        mPaintHistory.push(mCurrentPainter);
        std::cout << "paint history size: " << mPaintHistory.totalSize() << ", " << mPaintHistory.effectiveSize() << std::endl;
    } else {
        mCurrentPainter->onEnd(mbe.x, mbe.y);
    }
    repaint();
}

void EditorWindow::onKeyboardEvent(const KeyboardEvent& ke)
{
    AppWindow::onKeyboardEvent(ke);
    BLImageCodec codec;
    codec.findByName("BMP");
    windowBuffer().writeToFile("a.bmp", codec);
}

void EditorWindow::onMouseMoveEvent(const MouseMoveEvent& mme)
{
    if (mIsPainting) {
        mCurrentPainter->onDragging(mme.x, mme.y);
        repaint();
        return;
    }

    for (auto rit = mControls.rbegin(); rit != mControls.rend(); ++rit) {
        auto pControl = *rit;
        if (pControl->isInnerAbs(mme.x, mme.y)) {
            pControl->onMouseMoveEvent(mme);
            repaint();
            return;
        }
    }
    repaint();
}
