#include "window/AppWindow.h"
#include "window/AppWindowImpl.h"
#include <memory>

AppWindow::AppWindow()
    : mImpl(new AppWindow::Impl)
{
}

AppWindow::~AppWindow()
{
    delete mImpl;
}
void AppWindow::repaint()
{
    mImpl->mNeedRepaint = true;
}

void AppWindow::paint()
{
    // TODO prepare data to draw
}

void AppWindow::onPaint()
{
    paint();
    mImpl->onPaint();
}

bool AppWindow::initialize(const char* title, int w, int h)
{
    return mImpl->init(title, w, h, this);
}

bool AppWindow::needRepaint() const
{
    return mImpl->mNeedRepaint;
}

void AppWindow::setTitle(const char* title)
{
    mImpl->setTitle(title);
}

const char* AppWindow::title() const
{
    return mImpl->title();
}

void AppWindow::resize(int w, int h)
{
    mImpl->resize(w, h);
}

void AppWindow::onWindowEvent(const WindowEvent& we)
{
}

void AppWindow::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
}

void AppWindow::onKeyboardEvent(const KeyboardEvent& ke)
{
}

void AppWindow::onMouseMoveEvent(const MouseMoveEvent& mme)
{
}

void AppWindow::onTextInputEvent(const TextInputEvent& tie)
{
}

BLImage& AppWindow::windowBuffer()
{
    return *mImpl->mTextureBuf;
}

const BLImage& AppWindow::windowBuffer() const
{
    return *mImpl->mTextureBuf;
}
