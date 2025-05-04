module;

#include <cstring>
#include <Core/MacroUtils.hpp>

module Easy.Renderer.Renderer2D;

import Easy.Vendor.glm;

import Easy.Core.Basic;
import Easy.Renderer.Camera;
import Easy.Renderer.OrthographicCamera;
import Easy.Renderer.EditorCamera;
import Easy.Renderer.Texture;
import Easy.Renderer.Font;
import Easy.Scene.Scene;

import Easy.Renderer.VertexArray;
import Easy.Renderer.Buffer;
import Easy.Renderer.Shader;
import Easy.Renderer.UniformBuffer;

import Easy.Renderer.RenderCommand;

import Easy.Renderer.ShaderSources;

namespace Easy {
    struct QuadVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;

        // Editor-only
        int EntityID;
    };

    struct CircleVertex {
        glm::vec3 WorldPosition;
        glm::vec3 LocalPosition;
        glm::vec4 Color;
        float Thickness;
        float Fade;

        // Editor-only
        int EntityID;
    };

    struct LineVertex {
        glm::vec3 Position;
        glm::vec4 Color;

        // Editor-only
        int EntityID;
    };

    struct TextVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;

        // TODO: bg color for outline/bg

        // Editor-only
        int EntityID;
    };

    struct Renderer2DData {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

        Arc<VertexArray> QuadVertexArray;
        Arc<VertexBuffer> QuadVertexBuffer;
        Arc<Shader> QuadShader;
        Arc<Texture2D> WhiteTexture;

        Arc<VertexArray> CircleVertexArray;
        Arc<VertexBuffer> CircleVertexBuffer;
        Arc<Shader> CircleShader;

        Arc<VertexArray> LineVertexArray;
        Arc<VertexBuffer> LineVertexBuffer;
        Arc<Shader> LineShader;

        Arc<VertexArray> TextVertexArray;
        Arc<VertexBuffer> TextVertexBuffer;
        Arc<Shader> TextShader;

        uint32_t QuadIndexCount = 0;
        // QuadVertex *QuadVertexBufferBase = nullptr;
        // QuadVertex *QuadVertexBufferPtr = nullptr;
        std::vector<QuadVertex> QuadVertexArrayData;

        uint32_t CircleIndexCount = 0;
        // CircleVertex *CircleVertexBufferBase = nullptr;
        // CircleVertex *CircleVertexBufferPtr = nullptr;
        std::vector<CircleVertex> CircleVertexArrayData;

        uint32_t LineVertexCount = 0;
        // LineVertex *LineVertexBufferBase = nullptr;
        // LineVertex *LineVertexBufferPtr = nullptr;
        std::vector<LineVertex> LineVertexArrayData;

        uint32_t TextIndexCount = 0;
        // TextVertex *TextVertexBufferBase = nullptr;
        // TextVertex *TextVertexBufferPtr = nullptr;
        std::vector<TextVertex> TextVertexArrayData;

        float LineWidth = 2.0f;

        std::array<Arc<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture

        Arc<Texture2D> FontAtlasTexture;

        glm::vec4 QuadVertexPositions[4];

        Renderer2D::Statistics Stats;

        struct CameraData {
            glm::mat4 ViewProjection;
        };

        CameraData CameraBuffer;
        Arc<UniformBuffer> CameraUniformBuffer;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init() {
        // EZ_PROFILE_FUNCTION();

        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Float, "a_TexIndex"},
            {ShaderDataType::Float, "a_TilingFactor"},
            {ShaderDataType::Int, "a_EntityID"}
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        // s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
        s_Data.QuadVertexArrayData.reserve(s_Data.MaxVertices);

        // uint32_t *quadIndices = new uint32_t[s_Data.MaxIndices];
        std::vector<uint32_t> quadIndices;
        quadIndices.reserve(s_Data.MaxIndices);

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
            // quadIndices[i + 0] = offset + 0;
            quadIndices.push_back(offset + 0);
            // quadIndices[i + 1] = offset + 1;
            quadIndices.push_back(offset + 1);
            // quadIndices[i + 2] = offset + 2;
            quadIndices.push_back(offset + 2);

            // quadIndices[i + 3] = offset + 2;
            quadIndices.push_back(offset + 2);
            // quadIndices[i + 4] = offset + 3;
            quadIndices.push_back(offset + 3);
            // quadIndices[i + 5] = offset + 0;
            quadIndices.push_back(offset + 0);
            offset += 4;
        }

        Arc<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices.data(), s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        // delete[] quadIndices;

        // Circles
        s_Data.CircleVertexArray = VertexArray::Create();

        s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
        s_Data.CircleVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_WorldPosition"},
            {ShaderDataType::Float3, "a_LocalPosition"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float, "a_Thickness"},
            {ShaderDataType::Float, "a_Fade"},
            {ShaderDataType::Int, "a_EntityID"}
        });
        s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
        s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
        // s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
        s_Data.CircleVertexArrayData.reserve(s_Data.MaxVertices);

        // Lines
        s_Data.LineVertexArray = VertexArray::Create();

        s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
        s_Data.LineVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Int, "a_EntityID"}
        });
        s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
        s_Data.LineVertexArray->SetIndexBuffer(quadIB); // Use quad IB
        // s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];
        s_Data.LineVertexArrayData.reserve(s_Data.MaxVertices);

        // Text
        s_Data.TextVertexArray = VertexArray::Create();

        s_Data.TextVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(TextVertex));
        s_Data.TextVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Int, "a_EntityID"}
        });
        s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
        s_Data.TextVertexArray->SetIndexBuffer(quadIB);
        // s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];
        s_Data.TextVertexArrayData.reserve(s_Data.MaxVertices);

        s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;

        GLShaderSources::Init();

        s_Data.QuadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
        s_Data.CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
        s_Data.LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");
        s_Data.TextShader = Shader::Create("assets/shaders/Renderer2D_Text.glsl");

        // Set first texture slot to 0
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
    }

    void Renderer2D::Shutdown() {
        // EZ_PROFILE_FUNCTION();

        // delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera) {
        // EZ_PROFILE_FUNCTION();

        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

        StartBatch();
    }

    void Renderer2D::BeginScene(const Camera &camera, const glm::mat4 &transform) {
        // EZ_PROFILE_FUNCTION();

        s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

        StartBatch();
    }

    void Renderer2D::BeginScene(const EditorCamera &camera) {
        // EZ_PROFILE_FUNCTION();

        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

        StartBatch();
    }

    void Renderer2D::EndScene() {
        // EZ_PROFILE_FUNCTION();

        Flush();
    }

    void Renderer2D::StartBatch() {
        s_Data.QuadIndexCount = 0;
        // s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexArrayData.clear();

        s_Data.CircleIndexCount = 0;
        // s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
        s_Data.CircleVertexArrayData.clear();

        s_Data.LineVertexCount = 0;
        // s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
        s_Data.LineVertexArrayData.clear();

        s_Data.TextIndexCount = 0;
        // s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;
        s_Data.TextVertexArrayData.clear();
        s_Data.TextVertexArrayData.push_back({});

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::Flush() {
        if (s_Data.QuadIndexCount) {
            uint32_t dataSize = (uint32_t) (s_Data.QuadVertexArrayData.size() * sizeof(QuadVertex));
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexArrayData.data(), dataSize);

            // Bind textures
            for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
                s_Data.TextureSlots[i]->Bind(i);

            s_Data.QuadShader->Bind();
            RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
            s_Data.Stats.DrawCalls++;
        }

        if (s_Data.CircleIndexCount) {
            uint32_t dataSize = (uint32_t) (s_Data.CircleVertexArrayData.size() * sizeof(CircleVertex));
            s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexArrayData.data(), dataSize);

            s_Data.CircleShader->Bind();
            RenderCommand::DrawIndexed(s_Data.CircleVertexArray,
                                       s_Data.CircleIndexCount);
            s_Data.Stats.DrawCalls++;
        }

        if (s_Data.LineVertexCount) {
            // EZ_CORE_INFO("Line Vertex Count: {0}", s_Data.LineVertexCount);
            uint32_t dataSize = (uint32_t) (s_Data.LineVertexArrayData.size() * sizeof(LineVertex));
            // EZ_CORE_INFO("Line Vertex Size: {0}", dataSize);
            s_Data.LineVertexBuffer->SetData(s_Data.LineVertexArrayData.data(), dataSize);

            s_Data.LineShader->Bind();
            RenderCommand::SetLineWidth(s_Data.LineWidth);
            // EZ_CORE_ASSERT(s_Data.LineVertexArray, "Line Vertex Array is null");
            // EZ_CORE_ASSERT(s_Data.LineVertexArray->GetIndexBuffer(), "Line Vertex Array Index Buffer is null");
            RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
            s_Data.Stats.DrawCalls++;
        }

        if (s_Data.TextIndexCount) {
            uint32_t dataSize = (uint32_t) (s_Data.TextVertexArrayData.size() * sizeof(TextVertex));
            s_Data.TextVertexBuffer->SetData(s_Data.TextVertexArrayData.data(), dataSize);

            s_Data.FontAtlasTexture->Bind(0);

            s_Data.TextShader->Bind();
            RenderCommand::DrawIndexed(s_Data.TextVertexArray, s_Data.TextIndexCount);
            s_Data.Stats.DrawCalls++;
        }
    }

    void Renderer2D::NextBatch() {
        Flush();
        StartBatch();
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
        // EZ_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        DrawQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Arc<Texture2D> &texture,
                              float tilingFactor, const glm::vec4 &tintColor) {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Arc<Texture2D> &texture,
                              float tilingFactor, const glm::vec4 &tintColor) {
        // EZ_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        DrawQuad(transform, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color, int entityID) {
        // EZ_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;
        const float textureIndex = 0.0f; // White Texture
        constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
        const float tilingFactor = 1.0f;

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        for (size_t i = 0; i < quadVertexCount; i++) {
            s_Data.QuadVertexArrayData.push_back({
                .Position = transform * s_Data.QuadVertexPositions[i],
                .Color = color,
                .TexCoord = textureCoords[i],
                .TexIndex = textureIndex,
                .TilingFactor = tilingFactor,
                .EntityID = entityID
            });
        }

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawQuad(const glm::mat4 &transform, const Arc<Texture2D> &texture, float tilingFactor,
                              const glm::vec4 &tintColor, int entityID) {
        // EZ_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
            if (*s_Data.TextureSlots[i] == *texture) {
                textureIndex = (float) i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
                NextBatch();

            textureIndex = (float) s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        for (size_t i = 0; i < quadVertexCount; i++) {
            s_Data.QuadVertexArrayData.push_back({
                .Position = transform * s_Data.QuadVertexPositions[i],
                .Color = tintColor,
                .TexCoord = textureCoords[i],
                .TexIndex = textureIndex,
                .TilingFactor = tilingFactor,
                .EntityID = entityID
            });
        }

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                     const glm::vec4 &color) {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                     const glm::vec4 &color) {
        // EZ_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
                              * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        DrawQuad(transform, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                     const Arc<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                     const Arc<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) {
        // EZ_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
                              * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        DrawQuad(transform, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawCircle(const glm::mat4 &transform, const glm::vec4 &color, float thickness /*= 1.0f*/,
                                float fade /*= 0.005f*/, int entityID /*= -1*/) {
        // EZ_PROFILE_FUNCTION();

        // TODO: implement for circles
        // if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        	// NextBatch();

        for (size_t i = 0; i < 4; i++) {
            s_Data.CircleVertexArrayData.push_back({
                .WorldPosition = transform * s_Data.QuadVertexPositions[i],
                .LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f,
                .Color = color,
                .Thickness = thickness,
                .Fade = fade,
                .EntityID = entityID
            });
        }

        s_Data.CircleIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawLine(const glm::vec3 &p0, glm::vec3 &p1, const glm::vec4 &color, int entityID) {
        s_Data.LineVertexArrayData.push_back({
            .Position = p0,
            .Color = color,
            .EntityID = entityID
        });

        s_Data.LineVertexArrayData.push_back({
            .Position = p1,
            .Color = color,
            .EntityID = entityID
        });

        s_Data.LineVertexCount += 2;
    }

    void Renderer2D::DrawRect(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, int entityID) {
        glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
        glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
        glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
        glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

        DrawLine(p0, p1, color, entityID);
        DrawLine(p1, p2, color, entityID);
        DrawLine(p2, p3, color, entityID);
        DrawLine(p3, p0, color, entityID);
    }

    void Renderer2D::DrawRect(const glm::mat4 &transform, const glm::vec4 &color, int entityID) {
        glm::vec3 lineVertices[4];
        for (size_t i = 0; i < 4; i++)
            lineVertices[i] = transform * s_Data.QuadVertexPositions[i];

        DrawLine(lineVertices[0], lineVertices[1], color, entityID);
        DrawLine(lineVertices[1], lineVertices[2], color, entityID);
        DrawLine(lineVertices[2], lineVertices[3], color, entityID);
        DrawLine(lineVertices[3], lineVertices[0], color, entityID);
    }

    void Renderer2D::DrawSprite(const glm::mat4 &transform, SpriteRendererComponent &src, int entityID) {
        if (src.Texture)
            DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
        else
            DrawQuad(transform, src.Color, entityID);
    }

    void Renderer2D::DrawString(const std::string &string, Arc<Font> font, const glm::mat4 &transform,
                                const TextParams &textParams, int entityID) {
        const auto &fontGeometry = font->GetMSDFData()->FontGeometry;
        const auto &metrics = fontGeometry.getMetrics();
        Arc<Texture2D> fontAtlas = font->GetAtlasTexture();

        s_Data.FontAtlasTexture = fontAtlas;

        double x = 0.0;
        double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
        double y = 0.0;

        const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

        for (size_t i = 0; i < string.size(); i++) {
            char character = string[i];
            if (character == '\r')
                continue;

            if (character == '\n') {
                x = 0;
                y -= fsScale * metrics.lineHeight + textParams.LineSpacing;
                continue;
            }

            if (character == ' ') {
                float advance = spaceGlyphAdvance;
                if (i < string.size() - 1) {
                    char nextCharacter = string[i + 1];
                    double dAdvance;
                    fontGeometry.getAdvance(dAdvance, character, nextCharacter);
                    advance = (float) dAdvance;
                }

                x += fsScale * advance + textParams.Kerning;
                continue;
            }

            if (character == '\t') {
                // NOTE(Yan): is this right?
                x += 4.0f * (fsScale * spaceGlyphAdvance + textParams.Kerning);
                continue;
            }

            auto glyph = fontGeometry.getGlyph(character);
            if (!glyph)
                glyph = fontGeometry.getGlyph('?');
            if (!glyph)
                return;

            double al, ab, ar, at;
            glyph->getQuadAtlasBounds(al, ab, ar, at);
            glm::vec2 texCoordMin((float) al, (float) ab);
            glm::vec2 texCoordMax((float) ar, (float) at);

            double pl, pb, pr, pt;
            glyph->getQuadPlaneBounds(pl, pb, pr, pt);
            glm::vec2 quadMin((float) pl, (float) pb);
            glm::vec2 quadMax((float) pr, (float) pt);

            quadMin *= fsScale, quadMax *= fsScale;
            quadMin += glm::vec2(x, y);
            quadMax += glm::vec2(x, y);

            float texelWidth = 1.0f / fontAtlas->GetWidth();
            float texelHeight = 1.0f / fontAtlas->GetHeight();
            texCoordMin *= glm::vec2(texelWidth, texelHeight);
            texCoordMax *= glm::vec2(texelWidth, texelHeight);

            // render here
            s_Data.TextVertexArrayData.push_back({
                .Position = transform * glm::vec4(quadMin, 0.0f, 1.0f),
                .Color = textParams.Color,
                .TexCoord = texCoordMin,
                .EntityID = entityID
            });

            s_Data.TextVertexArrayData.push_back({
                .Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f),
                .Color = textParams.Color,
                .TexCoord = {texCoordMin.x, texCoordMax.y},
                .EntityID = entityID});

            s_Data.TextVertexArrayData.push_back({
                .Position = transform * glm::vec4(quadMax, 0.0f, 1.0f),
                .Color = textParams.Color,
                .TexCoord = texCoordMax,
                .EntityID = entityID
            });

            s_Data.TextVertexArrayData.push_back({
                .Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f),
                .Color = textParams.Color,
                .TexCoord = {texCoordMax.x, texCoordMin.y},
                .EntityID = entityID
            });

            s_Data.TextIndexCount += 6;
            s_Data.Stats.QuadCount++;

            if (i < string.size() - 1) {
                double advance = glyph->getAdvance();
                char nextCharacter = string[i + 1];
                fontGeometry.getAdvance(advance, character, nextCharacter);

                x += fsScale * advance + textParams.Kerning;
            }
        }
    }

    void Renderer2D::DrawString(const std::string &string, const glm::mat4 &transform, const TextComponent &component,
                                int entityID) {
        DrawString(string, component.FontAsset, transform, {component.Color, component.Kerning, component.LineSpacing},
                   entityID);
    }

    float Renderer2D::GetLineWidth() {
        return s_Data.LineWidth;
    }

    void Renderer2D::SetLineWidth(float width) {
        s_Data.LineWidth = width;
    }

    void Renderer2D::ResetStats() {
        std::memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_Data.Stats;
    }
}
