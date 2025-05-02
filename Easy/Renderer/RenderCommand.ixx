export module Easy.Renderer.RenderCommand;

import easy.vendor.glm;
import Easy.Core.Basic;
import Easy.Renderer.VertexArray;
import Easy.Renderer.RendererAPI;

namespace Easy {
    export class RenderCommand {
    public:
        static void Init() {
            s_RendererAPI = RendererAPI::Create();
            s_RendererAPI->Init();
        }

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        static void SetClearColor(const glm::vec4 &color) {
            s_RendererAPI->SetClearColor(color);
        }

        static void Clear() {
            s_RendererAPI->Clear();
        }

        static void DrawIndexed(const Arc<VertexArray> &vertexArray, uint32_t indexCount = 0) {
            s_RendererAPI->DrawIndexed(vertexArray, indexCount);
        }

        static void DrawLines(const Arc<VertexArray> &vertexArray, uint32_t vertexCount) {
            s_RendererAPI->DrawLines(vertexArray, vertexCount);
        }

        static void SetLineWidth(float width) {
            s_RendererAPI->SetLineWidth(width);
        }

    private:
        inline static Box<RendererAPI> s_RendererAPI;
    };
}
