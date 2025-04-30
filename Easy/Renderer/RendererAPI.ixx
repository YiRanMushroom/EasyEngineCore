module;

#include "Core/MacroUtils.hpp"

export module Easy.Renderer.RendererAPI;

import Easy.Core.Basic;
import Easy.Renderer.VertexArray;
import easy.vendor.glm;
import Easy.Renderer.Shader;

namespace Easy {
    export class RendererAPI;

    enum class API {
        None = 0, OpenGL = 1
    };

    class RendererAPI {
    public:
        using API = API;

    public:
        virtual ~RendererAPI() = default;

        virtual void Init() = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void SetClearColor(const glm::vec4 &color) = 0;

        virtual void Clear() = 0;

        virtual void DrawIndexed(const Arc<VertexArray> &vertexArray, uint32_t indexCount = 0) = 0;

        virtual void DrawLines(const Arc<VertexArray> &vertexArray, uint32_t vertexCount) = 0;

        virtual void SetLineWidth(float width) = 0;

        virtual API GetInstanceAPI() = 0;

        virtual Arc<Shader> CreateShader(const std::string &name, const std::string &vertexSrc,
                                         const std::string &fragmentSrc);

        virtual Arc<Shader> CreateShader(const std::string &filepath);

        static API GetAPI() { return s_CurrentRendererAPI->GetInstanceAPI(); }

        static Box<RendererAPI> Create();

    protected:
        inline static RendererAPI *s_CurrentRendererAPI;
    };

    namespace Renderer {
        export API GetAPI() {
            return RendererAPI::GetAPI();
        }
    }
}
