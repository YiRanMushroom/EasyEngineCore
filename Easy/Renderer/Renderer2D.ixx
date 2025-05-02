export module Easy.Renderer.Renderer2D;

import Easy.Vendor.glm;

import Easy.Core.Basic;
import Easy.Renderer.Camera;
import Easy.Renderer.OrthographicCamera;
import Easy.Renderer.EditorCamera;
import Easy.Renderer.Texture;
import Easy.Renderer.Font;
import Easy.Scene.Components;

namespace Easy {
    export namespace Renderer2D {
        void Init();

        void Shutdown();

        void BeginScene(const Camera &camera, const glm::mat4 &transform);

        void BeginScene(const EditorCamera &camera);

        void BeginScene(const OrthographicCamera &camera); // TODO: Remove
        void EndScene();

        void Flush();

        // Primitives
        void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);

        void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);

        void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Arc<Texture2D> &texture,
                      float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Arc<Texture2D> &texture,
                      float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        void DrawQuad(const glm::mat4 &transform, const glm::vec4 &color, int entityID = -1);

        void DrawQuad(const glm::mat4 &transform, const Arc<Texture2D> &texture, float tilingFactor = 1.0f,
                      const glm::vec4 &tintColor = glm::vec4(1.0f), int entityID = -1);

        void DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                             const glm::vec4 &color);

        void DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                             const glm::vec4 &color);

        void DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                             const Arc<Texture2D> &texture, float tilingFactor = 1.0f,
                             const glm::vec4 &tintColor = glm::vec4(1.0f));

        void DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                             const Arc<Texture2D> &texture, float tilingFactor = 1.0f,
                             const glm::vec4 &tintColor = glm::vec4(1.0f));

        void DrawCircle(const glm::mat4 &transform, const glm::vec4 &color, float thickness = 1.0f,
                        float fade = 0.005f, int entityID = -1);

        void DrawLine(const glm::vec3 &p0, glm::vec3 &p1, const glm::vec4 &color, int entityID = -1);

        void DrawRect(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color,
                      int entityID = -1);

        void DrawRect(const glm::mat4 &transform, const glm::vec4 &color, int entityID = -1);

        void DrawSprite(const glm::mat4 &transform, SpriteRendererComponent &src, int entityID);

        struct TextParams {
            glm::vec4 Color{1.0f};
            float Kerning = 0.0f;
            float LineSpacing = 0.0f;
        };

        void DrawString(const std::string &string, Arc<Font> font, const glm::mat4 &transform,
                        const TextParams &textParams, int entityID = -1);

        void DrawString(const std::string &string, const glm::mat4 &transform, const TextComponent &component,
                        int entityID = -1);

        float GetLineWidth();

        void SetLineWidth(float width);

        // Stats
        struct Statistics {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            [[nodiscard]] uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            [[nodiscard]] uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };

        void ResetStats();

        Statistics GetStats();
    }

    namespace Renderer2D {
        void StartBatch();

        void NextBatch();
    }
}
