#pragma once
#include <string>

class Painter;

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

    const std::string& name() const
    {
        return mName;
    }

    void setName(const std::string& name)
    {
        mName = name;
    }

    virtual void paint(Painter& painter) = 0;

protected:
    std::string mName;
    int mX;
    int mY;
    int mWidth;
    int mHeight;
};