export module Easy.Events.MouseCodes;

import yrm.msvc.std.compat;

namespace Easy {
    export enum class MouseCode : uint16_t {
        Button0 = 0,
        Button1, Button2, Button3, Button4, Button5, Button6, Button7,
        ButtonLast = Button7,
        ButtonLeft = Button0,
        ButtonRight = Button1,
        ButtonMiddle = Button2
    };
}
