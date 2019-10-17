#pragma once
#include "editor/ctrl/Control.h"
#include <memory>
#include <vector>

class Container : public Control {
public:
    using ControlRef = std::shared_ptr<Control>;

    void addChild(const ControlRef& c);

    bool removeChild(size_t index);

    bool removeChild(const std::string& name);

    ControlRef childAt(size_t index);

    const std::vector<ControlRef>& children() const;

    size_t childrenCount() const;


protected:
    void paint(Painter& painter) override;
    void onMouseButtonEvent(const MouseButtonEvent& mbe) override;
    void onKeyboardEvent(const KeyboardEvent& ke) override;
    void onMouseMoveEvent(const MouseMoveEvent& mme) override;

protected:
    std::vector<ControlRef> mChildren;

    friend class EditorWindow;
};
