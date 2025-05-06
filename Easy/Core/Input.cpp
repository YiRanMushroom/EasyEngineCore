module;

#include "MacroUtils.hpp"

module Easy.Core.Input;

import Easy.Core.Window;
import Easy.Core.Basic;

#ifdef EZ_PLATFORM_WINDOWS
import Easy.Platform.Impl.Windows.WindowsInput;
#endif

namespace Easy {
    void Input::Init() {
        switch (WindowAPI::GetAPI()) {
            case WindowAPI::API::None:
                EZ_CORE_ASSERT(false, "No window API selected!");
                break;
            case WindowAPI::API::Windows:
#ifdef EZ_PLATFORM_WINDOWS
                s_Input = MakeArc<WindowsInput>();
#else
                EZ_CORE_ASSERT(false, "Windows platform not supported!");
#endif
                break;
            default:
                EZ_CORE_ASSERT(false, "Unknown window API selected!");
                break;
        }
    }
}
