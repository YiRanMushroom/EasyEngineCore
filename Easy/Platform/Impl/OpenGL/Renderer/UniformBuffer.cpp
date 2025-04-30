module;

#include <OpenGL.hpp>

module Easy.Platform.Impl.OpenGL.Renderer.UniformBuffer;

import Easy.Core.Basic;
import Easy.Renderer.UniformBuffer;

namespace Easy {
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) { {
            uint32_t id;
            glCreateBuffers(1, &id);
            m_RendererID = {id};
        }
        glNamedBufferData(m_RendererID.get(), size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID.get());
    }


    void OpenGLUniformBuffer::SetData(const void *data, uint32_t size, uint32_t offset) {
        glNamedBufferSubData(m_RendererID.get(), offset, size, data);
    }

    void OpenGLUniformBuffer::UniformBufferDeleter::operator()(uint32_t buffer) const {
        glDeleteBuffers(1, &buffer);
    }
}
