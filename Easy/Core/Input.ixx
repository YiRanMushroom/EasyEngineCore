export module Easy.Core.Input;

import Easy.Core.Basic;
import Easy.Core.KeyCodes;
import Easy.Core.MouseCodes;
import Easy.Vendor.glm;

namespace Easy {
    export class Input {
    public:
        static bool IsKeyPressed(Key::KeyCode key);

        static bool IsMouseButtonPressed(Mouse::MouseCode button);

        static glm::vec2 GetMousePosition();

        static float GetMouseX();

        static float GetMouseY();
    };
}
