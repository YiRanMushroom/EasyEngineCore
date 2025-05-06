module;

#include <OpenGL.hpp>

export module Easy.Platform.Impl.OpenGL.Renderer.Texture;

import Easy.Core.Basic;
import Easy.Renderer.Texture;
import Easy.Core.Util;
import Easy.Renderer.RendererAPI;

namespace Easy {
    export class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const TextureSpecification &specification);

        OpenGLTexture2D(const std::string &path);

        virtual ~OpenGLTexture2D() = default;

        virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }

        virtual const std::string &GetPath() const override { return m_Path; }

        virtual void SetData(void *data, uint32_t size) override;

        virtual void Bind(uint32_t slot = 0) const override;

        virtual bool IsLoaded() const override { return m_IsLoaded; }

        virtual bool operator==(const Texture &other) const override {
            if (other.GetAPI() != Easy::RendererAPI::API::OpenGL)
                return false;
            return m_RendererID == static_cast<const OpenGLTexture2D &>(other).m_RendererID;
        }

        virtual Easy::RendererAPI::API GetAPI() const override {
            return Easy::RendererAPI::API::OpenGL;
        }

        virtual uint32_t GetRendererID() const override {
            return m_RendererID.get();
        }

    private:
        struct TextureDeleter {
            void operator()(uint32_t textureID);
        };

        TextureSpecification m_Specification;

        std::string m_Path;
        bool m_IsLoaded = false;
        uint32_t m_Width, m_Height;
        unique_owner_default<uint32_t, TextureDeleter, 0> m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
}
