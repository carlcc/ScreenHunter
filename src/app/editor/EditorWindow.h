#pragma once

#include "editor/ctrl/ColorPicker.h"
#include "editor/ctrl/ToolPicker.h"
#include "editor/paint/PaintHistory.h"
#include "window/AppWindow.h"

class EditorWindow : public AppWindow {
public:
    EditorWindow();
    ~EditorWindow();

protected:
    void paint() override;
    void onWindowEvent(const WindowEvent& we) override;
    void onMouseButtonEvent(const MouseButtonEvent& mbe) override;
    void onKeyboardEvent(const KeyboardEvent& ke) override;
    void onMouseMoveEvent(const MouseMoveEvent& mme) override;

private:
    std::vector<std::shared_ptr<Control>> mControls;
    ColorPicker* mColorButtons;
    ToolPicker* mToolButtons;
    std::shared_ptr<IElementPainter> mCurrentPainter { nullptr };
    PaintHistory mPaintHistory;
    bool mIsPainting { false };
    friend class App;
};