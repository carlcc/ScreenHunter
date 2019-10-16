#include "Container.h"

void Container::addChild(const ControlRef& c)
{
    mChildren.push_back(c);
}

bool Container::removeChild(size_t index)
{
    if (index >= mChildren.size()) {
        return false;
    }

    for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
        if (index-- == 0) {
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
    Control::paint(painter);

    for (auto& c : mChildren) {
        c->paint(painter);
    }
}
