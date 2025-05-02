module;

export module Easy.Renderer.Renderer;

import Easy.Vendor.glm;
import Easy.Core.Basic;
export import Easy.Renderer.RendererAPI;
import Easy.Renderer.OrthographicCamera;
import Easy.Renderer.Shader;
import Easy.Renderer.VertexArray;

namespace Easy {
    namespace Renderer {
        export void Init();

        export void Shutdown();

        export void OnWindowResize(uint32_t width, uint32_t height);

        export void BeginScene(OrthographicCamera &camera);

        export void EndScene();

        export void Submit(const Arc<Shader> &shader, const Arc<VertexArray> &vertexArray,
                           const glm::mat4 &transform = glm::mat4(1.0f));

        struct SceneData {
            glm::mat4 ViewProjectionMatrix;
        };

        export Box<SceneData> s_SceneData = MakeBox<SceneData>();
    };
}
