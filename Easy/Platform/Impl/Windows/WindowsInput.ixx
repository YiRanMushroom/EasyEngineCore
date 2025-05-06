module;

#include <OpenGL.hpp>

export module Easy.Platform.Impl.Windows.WindowsInput;

import Easy.Core.Input;
import Easy.Core.Basic;
import Easy.Core.Application;
import Easy.Core.KeyCodes;
import Easy.Core.MouseCodes;
import Easy.Vendor.glm;

namespace Easy {
    export class WindowsInput : public Input {
        bool IsKeyPressed(const Key::KeyCode key) override {
            auto *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
            auto state = glfwGetKey(window, static_cast<int32_t>(key));
            return state == GLFW_PRESS;
        }

        bool IsMouseButtonPressed(const Mouse::MouseCode button) override {
            auto *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
            auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
            return state == GLFW_PRESS;
        }

        glm::vec2 GetMousePosition() override {
            auto *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            return {(float) xpos, (float) ypos};
        }

        float GetMouseX() override {
            return GetMousePosition().x;
        }

        float GetMouseY() override {
            return GetMousePosition().y;
        }
    };
}
