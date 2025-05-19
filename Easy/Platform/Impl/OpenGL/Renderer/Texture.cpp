module;

#include <stb_image.h>
#include <OpenGL.hpp>
#include "Core/MacroUtils.hpp"
#include "glm/fwd.hpp"

module Easy.Platform.Impl.OpenGL.Renderer.Texture;
import Easy.Core.Basic;
import Easy.Core.Log;


namespace Easy {
    namespace Utils {
        static GLenum HazelImageFormatToGLDataFormat(ImageFormat format) {
            switch (format) {
                case ImageFormat::RGB8: return GL_RGB;
                case ImageFormat::RGBA8: return GL_RGBA;
                default: EZ_CORE_ASSERT(false, "Unknown ImageFormat!");
                    return 0;
            }

            EZ_CORE_ASSERT(false, "Unknown ImageFormat!");
            return 0;
        }

        static GLenum HazelImageFormatToGLInternalFormat(ImageFormat format) {
            switch (format) {
                case ImageFormat::RGB8: return GL_RGB8;
                case ImageFormat::RGBA8: return GL_RGBA8;
                default: EZ_CORE_ASSERT(false, "Unknown ImageFormat!");
                    return 0;
            }

            EZ_CORE_ASSERT(false);
            return 0;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification &specification)
        : m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height) {
        // HZ_PROFILE_FUNCTION();

        m_InternalFormat = Utils::HazelImageFormatToGLInternalFormat(m_Specification.Format);
        m_DataFormat = Utils::HazelImageFormatToGLDataFormat(m_Specification.Format);
        // Create OpenGL texture
        {
            uint32_t id = 0;
            glCreateTextures(GL_TEXTURE_2D, 1, &id);
            m_RendererID = {id};
        }
        glTextureStorage2D(m_RendererID.get(), 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID.get(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID.get(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID.get(), GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID.get(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string &path)
        : m_Path(path) {
        // HZ_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = nullptr; {
            // HZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }

        if (data) {
            m_IsLoaded = true;

            m_Width = width;
            m_Height = height;

            GLenum internalFormat = 0, dataFormat = 0;
            if (channels == 4) {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            } else if (channels == 3) {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }

            m_InternalFormat = internalFormat;
            m_DataFormat = dataFormat;

            // HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

            {
                uint32_t id = 0;
                glCreateTextures(GL_TEXTURE_2D, 1, &id);
                m_RendererID = {id};
            }

            glTextureStorage2D(m_RendererID.get(), 1, internalFormat, m_Width, m_Height);

            glTextureParameteri(m_RendererID.get(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID.get(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTextureParameteri(m_RendererID.get(), GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_RendererID.get(), GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D(m_RendererID.get(), 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
    }

    /*OpenGLTexture2D::~OpenGLTexture2D() {
        // HZ_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }*/

    void OpenGLTexture2D::SetData(void *data, uint32_t size) {
        // HZ_PROFILE_FUNCTION();

        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        EZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID.get(), 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const {
        // HZ_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_RendererID.get());
    }

    void OpenGLTexture2D::TextureDeleter::operator()(uint32_t textureID) {
        EZ_CORE_INFO("Deleting OpenGL texture {0}", textureID);
        glDeleteTextures(1, &textureID);
    }
}
