export module Easy.Platform.Impl.OpenGL.Renderer.RendererAPI;

import Easy.Renderer.RendererAPI;
import Easy.Renderer.Shader;
import Easy.Renderer.VertexArray;
import Easy.Core.Basic;
import Easy.Vendor.glm;

namespace Easy {
    export class OpenGLRendererAPI : public RendererAPI {
    public:
        OpenGLRendererAPI();

        ~OpenGLRendererAPI() override;

        virtual void Init() override;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        virtual void SetClearColor(const glm::vec4 &color) override;

        virtual void Clear() override;

        virtual void DrawIndexed(const Arc<VertexArray> &vertexArray, uint32_t indexCount = 0) override;

        virtual void DrawLines(const Arc<VertexArray> &vertexArray, uint32_t vertexCount) override;

        virtual void SetLineWidth(float width) override;

        virtual API GetInstanceAPI() override { return API::OpenGL; }

        virtual Arc<Shader> CreateShader(const std::string &name, const std::string &vertexSrc,
                                         const std::string &fragmentSrc) override;

        virtual Arc<Shader> CreateShader(const std::string &filepath) override;
    };

}
