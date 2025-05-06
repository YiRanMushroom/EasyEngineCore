module;
#include "MacroUtils.hpp"
export module Easy.Core.Input;

import Easy.Core.Basic;
import Easy.Core.KeyCodes;
import Easy.Core.MouseCodes;
import Easy.Vendor.glm;

namespace Easy {
    export class Input {
    public:
        virtual ~Input() = default;

        virtual bool IsKeyPressed(Key::KeyCode key) = 0;

        virtual bool IsMouseButtonPressed(Mouse::MouseCode button) = 0;

        virtual glm::vec2 GetMousePosition() = 0;

        virtual float GetMouseX() = 0;

        virtual float GetMouseY() = 0;

        static const Arc<Input>& Get() {
            EZ_CORE_ASSERT(s_Input, "Input is not initialized!");
            return s_Input;
        }

        static void Init();

        static void Shutdown() {
            s_Input.reset();
        }

    private:
        inline static Arc<Input> s_Input;
    };
}
