module;

module Easy.Renderer.Renderer;

import Easy.Vendor.glm;
import Easy.Core.Basic;
import Easy.Renderer.RendererAPI;
import Easy.Renderer.OrthographicCamera;
import Easy.Renderer.Shader;
import Easy.Renderer.VertexArray;
import Easy.Renderer.Renderer2D;
import Easy.Renderer.RenderCommand;

namespace Easy {
    void Renderer::Init() {
        // HZ_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown() {
        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera &camera) {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {}

    void Renderer::Submit(const Arc<Shader> &shader, const Arc<VertexArray> &vertexArray, const glm::mat4 &transform) {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
