#include "editor/ctrl/Container.h"
#include "event/KeyboardEvent.h"
#include "event/MouseButtonEvent.h"
#include "event/MouseMoveEvent.h"

void Container::addChild(const ControlRef& c)
{
    mChildren.push_back(c);
    c->mParent = this;
}

bool Container::removeChild(size_t index)
{
    if (index >= mChildren.size()) {
        return false;
    }

    for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
        if (index-- == 0) {
            it->get()->mParent = nullptr;
            mChildren.erase(it);
            return true;
        }
    }

    abort(); // impossible
}

bool Container::removeChild(const std::string& name)
{
    for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
        if ((*it)->name() == name) {
            it->get()->mParent = nullptr;
            mChildren.erase(it);
            return true;
        }
    }
    return false;
}

Container::ControlRef Container::childAt(size_t index)
{
    if (index > mChildren.size()) {
        return nullptr;
    }
    return mChildren[index];
}

const std::vector<Container::ControlRef>& Container::children() const
{
    return mChildren;
}

size_t Container::childrenCount() const
{
    return mChildren.size();
}

void Container::paint(Painter& painter)
{
    if (visible()) {
        Control::paint(painter);

        for (auto& c : mChildren) {
            c->paint(painter);
        }
    }
}
void Container::onMouseButtonEvent(const MouseButtonEvent& mbe)
{
    for (auto& c : mChildren) {
        if (c->isInnerAbs(mbe.x, mbe.y)) {
            c->onMouseButtonEvent(mbe);
        }
    }
}

void Container::onKeyboardEvent(const KeyboardEvent& ke)
{
    Control::onKeyboardEvent(ke);
}

void Container::onMouseMoveEvent(const MouseMoveEvent& mme)
{
    for (auto& c : mChildren) {
        if (c->isInnerAbs(mme.x, mme.y)) {
            c->onMouseMoveEvent(mme);
        }
    }
}

void Container::markCoordDirty()
{
    mIsAbsCoordDirty = true;
    for (auto& c : mChildren) {
        c->markCoordDirty();
    }
}
