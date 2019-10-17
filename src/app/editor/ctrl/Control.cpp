#include "editor/ctrl/Control.h"
#include "editor/ctrl/Container.h"
#include "window/Painter.h"

void Control::paint(Painter& painter)
{
}
void Control::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
}
void Control::onKeyboardEvent(const KeyboardEvent& ke)
{
}
void Control::onMouseMoveEvent(const MouseMoveEvent& mme)
{
}

void Control::updateAbsCoord() const
{
    if (mIsAbsCoordDirty) {
        mIsAbsCoordDirty = false;
        if (mParent != nullptr) {
            mAbsX = mParent->absX() + x();
            mAbsY = mParent->absY() + y();
        } else {
            mAbsX = x();
            mAbsY = y();
        }
    }
}

