module Easy.Renderer.OrthographicCameraController;

import Easy.Vendor.glm;
import Easy.Core.Basic;
import Easy.Core.Application;
import Easy.Core.Input;
import Easy.Events.MouseEvents;
import Easy.Events.KeyEvents;
import Easy.Events.ApplicationEvents;
import Easy.Renderer.OrthographicCamera;

namespace Easy {
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio),
          m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
          m_Rotation(rotation) {}

    void OrthographicCameraController::OnUpdate(float ts) {
        // EZ_PROFILE_FUNCTION();

        if (Input::Get()->IsKeyPressed(Key::A)) {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        } else if (Input::Get()->IsKeyPressed(Key::D)) {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        if (Input::Get()->IsKeyPressed(Key::W)) {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        } else if (Input::Get()->IsKeyPressed(Key::S)) {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        if (m_Rotation) {
            if (Input::Get()->IsKeyPressed(Key::Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            if (Input::Get()->IsKeyPressed(Key::E))
                m_CameraRotation -= m_CameraRotationSpeed * ts;

            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f)
                m_CameraRotation += 360.0f;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event &e) {
        // EZ_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        // dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        // dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent &evt) {
        //     return OnMouseScrolled(evt);
        // });
        dispatcher.Dispatch(&OrthographicCameraController::OnMouseScrolled, this);
        // dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
        // dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent &evt) {
        //     return OnWindowResized(evt);
        // });
        dispatcher.Dispatch(&OrthographicCameraController::OnWindowResized, this);
    }

    void OrthographicCameraController::OnResize(float width, float height) {
        m_AspectRatio = width / height;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &e) {
        // EZ_PROFILE_FUNCTION();

        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &e) {
        // EZ_PROFILE_FUNCTION();

        OnResize((float) e.GetWidth(), (float) e.GetHeight());
        return false;
    }
}
