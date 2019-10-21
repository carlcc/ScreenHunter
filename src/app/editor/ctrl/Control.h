#pragma once
#include <memory>
#include <string>

class Painter;
class Container;

class MouseButtonEvent;
class KeyboardEvent;
class MouseMoveEvent;

class Control {
public:
    Control() = default;
    ~Control() = default;

    Control(const Control&) = delete;
    Control(Control&&) = delete;
    Control& operator=(const Control&) = delete;
    Control& operator=(Control&&) = delete;

    int x() const
    {
        return mX;
    }

    int y() const
    {
        return mY;
    }

    void setPosition(int x, int y)
    {
        mX = x;
        mY = y;
        markCoordDirty();
    }

    int width() const
    {
        return mWidth;
    }

    int height() const
    {
        return mHeight;
    }

    void setSize(int w, int h)
    {
        mWidth = w;
        mHeight = h;
    }

    int absX() const
    {
        updateAbsCoord();
        return mAbsX;
    }

    int absY() const
    {
        updateAbsCoord();
        return mAbsY;
    }

    const std::string& name() const
    {
        return mName;
    }

    void setName(const std::string& name)
    {
        mName = name;
    }

    Container* parent() const
    {
        return mParent;
    }

    bool visible() const
    {
        return mIsVisible;
    }

    void setVisible(bool b)
    {
        mIsVisible = b;
    }

    bool isInnerAbs(int x, int y)
    {
        return x >= absX() && y >= absY() && x < absX() + width() && y < absY() + height();
    }

    virtual void markCoordDirty() {
        mIsAbsCoordDirty = true;
    }

protected:
    virtual void paint(Painter& painter) = 0;
    virtual void onMouseButtonEvent(const MouseButtonEvent& mbe);
    virtual void onKeyboardEvent(const KeyboardEvent& ke);
    virtual void onMouseMoveEvent(const MouseMoveEvent& mme);

private:
    void updateAbsCoord() const;

protected:
    Container* mParent { nullptr };
    std::string mName { "" };
    int mX { 0 };
    int mY { 0 };
    int mWidth { 0 };
    int mHeight { 0 };
    bool mIsVisible { true };
    mutable bool mIsAbsCoordDirty { true };
    mutable int mAbsX { 0 };
    mutable int mAbsY { 0 };

    friend class Container;
    friend class EditorWindow;
};