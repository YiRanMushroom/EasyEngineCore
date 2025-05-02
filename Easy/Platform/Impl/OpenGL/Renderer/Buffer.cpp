module;

#include <glad/glad.h>

#include "Core/MacroUtils.hpp"

module Easy.Platform.Impl.OpenGL.Renderer.Buffer;

import Easy.Renderer.Buffer;
import Easy.Vendor.glm;

namespace Easy {
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) { {
            uint32_t id;
            glGenBuffers(1, &id);
            m_RendererID = {id};
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID.get());
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float *vertices, uint32_t size) { {
            uint32_t id;
            glGenBuffers(1, &id);
            m_RendererID = {id};
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID.get());
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void OpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID.get());
    }

    void OpenGLVertexBuffer::Unbind() const {
#if defined(EZ_DEBUG)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    }

    void OpenGLVertexBuffer::SetData(const void *data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID.get());
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count) : m_Count(count) {
    // scope
        {
            uint32_t id;
            glGenBuffers(1, &id);
            m_RendererID = {id};
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID.get());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    void OpenGLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID.get());
    }

    void OpenGLIndexBuffer::Unbind() const {
#if defined(EZ_DEBUG)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
    }

    void OpenGLVertexBuffer::VertexBufferDeleter::operator()(uint32_t ptr) const {
        EZ_CORE_INFO("Deleting VertexBuffer: {0}", ptr);
        glDeleteBuffers(1, &ptr);
    }

    void OpenGLIndexBuffer::IndexBufferDeleter::operator()(uint32_t ptr) const {
        EZ_CORE_INFO("Deleting IndexBuffer: {0}", ptr);
        glDeleteBuffers(1, &ptr);
    }
}
