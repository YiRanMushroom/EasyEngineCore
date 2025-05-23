module;

#include "MacroUtils.hpp"
#include "GLFW/glfw3.h"

export module Easy.Core.Window;

import Easy.Core.Basic;
import Easy.Events.Event;

namespace Easy {
    export class WindowProperties {
    public:
        std::string Title;
        int Width;
        int Height;

        explicit WindowProperties(std::string title = "Easy Engine", int width = 1600, int height = 900)
            : Title(std::move(title)), Width(width), Height(height) {
        }
    };

    export class WindowAPI {
    public:
        enum class API {
            None = 0,
            Windows = 1,
            MacOS = 2,
            Linux = 3,
        };

        inline static API s_API;

        static WindowAPI::API GetAPI() {
            return s_API;
        }
    };

    export class Window {
    public:
        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        [[nodiscard]] virtual uint32_t GetWidth() const = 0;

        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        [[nodiscard]] virtual std::pair<int, int> GetSize() const {
            return {GetWidth(), GetHeight()};
        }

        virtual void SetVSync(bool enabled) = 0;
        [[nodiscard]] virtual bool IsVSync() const = 0;

        virtual void SetEventCallback(std::function<void(Event &)> callback) = 0;

        [[nodiscard]] virtual GLFWwindow *GetNativeWindow() const = 0;
    };
}
