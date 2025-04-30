export module Easy.Renderer.Camera;

import easy.vendor.glm;

namespace Easy {
    export class Camera {
    public:
        Camera() = default;

        Camera(const glm::mat4 &projection)
            : m_Projection(projection) {}

        virtual ~Camera() = default;

        const glm::mat4 &GetProjection() const { return m_Projection; }

    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
    };
}
