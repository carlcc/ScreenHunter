#include "window/ctrl/Control.h"
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

    void paint(Painter& painter) override;

protected:
    std::vector<ControlRef> mChildren;
};
