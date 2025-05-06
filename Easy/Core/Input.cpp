module;

#include "MacroUtils.hpp"

module Easy.Core.Input;

import Easy.Core.Window;
import Easy.Core.Basic;

#ifdef EZ_PLATFORM_WINDOWS
import Easy.Platform.Impl.Windows.WindowsInput;
#endif

namespace Easy {
    bool Input::IsKeyPressed(Key::KeyCode key) {
        return InputCommand::Get()->IsKeyPressed(key);
    }

    bool Input::IsMouseButtonPressed(Mouse::MouseCode button) {
        return InputCommand::Get()->IsMouseButtonPressed(button);
    }

    glm::vec2 Input::GetMousePosition() {
        return InputCommand::Get()->GetMousePosition();
    }

    float Input::GetMouseX() {
        return InputCommand::Get()->GetMouseX();
    }

    float Input::GetMouseY() {
        return InputCommand::Get()->GetMouseY();
    }


    void Input::Init() {
        switch (WindowAPI::GetAPI()) {
            case WindowAPI::API::None:
                EZ_CORE_ASSERT(false, "No window API selected!");
                break;
            case WindowAPI::API::Windows:
#ifdef EZ_PLATFORM_WINDOWS
                InputCommand::s_InputCommand = MakeArc<WindowsInput>();
#else
                EZ_CORE_ASSERT(false, "Windows platform not supported!");
#endif
                break;
            default:
                EZ_CORE_ASSERT(false, "Unknown window API selected!");
                break;
        }
    }

    void Input::Shutdown() {
        InputCommand::s_InputCommand.reset();
    }
}
