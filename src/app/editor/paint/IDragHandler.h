#pragma once

class IDragHandler {
public:
    virtual void onStart(int x, int y) = 0;

    virtual void onEnd(int x, int y) = 0;

    virtual void onDragging(int x, int y) = 0;
};
