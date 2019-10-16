#pragma once

class MouseButtonEvent {
public:
    enum Button {
        BButtonLeft,
        BButtonRight,
        BButtonMid,
        BButtonX1,
        BButtonX2,
    };
    enum Action {
        ADown,
        AUp
    };

    Button button;
    Action action;
    int x, y;
};