//
// Created by carlcc on 2019/10/16.
//

#include "editor/EditorWindow.h"
#include "event/KeyboardEvent.h"
#include "event/MouseButtonEvent.h"
#include "event/MouseMoveEvent.h"
#include "window/Painter.h"
#include <iostream>

EditorWindow::EditorWindow()
{
    mButton1.setSize(100, 100);
    mButton1.setPosition(100, 100);
    mButton1.setFillColor(BLRgba32(0xff0000ff));
    mButton1.setBorderColor(BLRgba32(0xffff0000));
    mButton2.setSize(100, 100);
    mButton2.setPosition(200, 100);
    mButton2.setFillColor(BLRgba32(0xff00ff00));
    mButton2.setBorderColor(BLRgba32(0x00ffff00));
    mButton3.setSize(100, 100);
    mButton3.setPosition(100, 200);
    mButton3.setFillColor(BLRgba32(0x00ff00ff));
    mButton3.setBorderColor(BLRgba32(0x0000ffff));
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
    painter.setStrokeWidth(4);

    mButton1.paint(painter);
    mButton2.paint(painter);
    mButton3.paint(painter);

    std::cout << "Paint " << std::endl;
}

void EditorWindow::onWindowEvent(const WindowEvent& we)
{
    AppWindow::onWindowEvent(we);
}

void EditorWindow::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
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
    repaint();
}
