module;
export module Easy.Core.Input;

import <Core/MacroUtils.hu>;
import Easy.Core.Basic;
import Easy.Core.KeyCodes;
import Easy.Core.MouseCodes;
import Easy.Vendor.glm;

namespace Easy {
    export class InputCommand {
    public:
        virtual ~InputCommand() = default;
        virtual bool IsKeyPressed(Key::KeyCode key) = 0;
        virtual bool IsMouseButtonPressed(Mouse::MouseCode button) = 0;
        virtual glm::vec2 GetMousePosition() = 0;
        virtual float GetMouseX() = 0;
        virtual float GetMouseY() = 0;

        static const Arc<InputCommand>& Get() {
            EZ_CORE_ASSERT(s_InputCommand, "Input is not initialized!");
            return s_InputCommand;
        }

    public:
        inline static Arc<InputCommand> s_InputCommand;
    };

    export namespace Input {
        bool IsKeyPressed(Key::KeyCode key);

        bool IsMouseButtonPressed(Mouse::MouseCode button);

        glm::vec2 GetMousePosition();

        float GetMouseX();

        float GetMouseY();

        void Init();

        void Shutdown();
    };
}
