module;

import <Core/MacroUtils.hpp>;
import <Platform/Impl/OpenGL/GLFW_OpenGL.hpp>;

module Easy.Platform.Impl.OpenGL.Renderer.VertexArray;

import Easy.Core.Basic;
import Easy.Core.Log;

namespace Easy {
    OpenGLVertexArray::OpenGLVertexArray() {
        // scope
        {
            uint32_t id;
            glGenVertexArrays(1, &id);
            m_RendererID = {id};
        }
    }

    void OpenGLVertexArray::Bind() const {
        glBindVertexArray(m_RendererID.get());
    }

    void OpenGLVertexArray::Unbind() const {
#if defined(EZ_DEBUG)
        glBindVertexArray(0);
#endif
    }

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
            default:
                EZ_ASSERT(false, "Unknown ShaderDataType!");
        }

        return 0;
    }

    void OpenGLVertexArray::AddVertexBuffer(const Arc<VertexBuffer> &vertexBuffer) {
        EZ_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout!");
        glBindVertexArray(m_RendererID.get());
        vertexBuffer->Bind();
        const auto &layout = vertexBuffer->GetLayout();

        for (const auto &element: layout) {
            switch (element.Type) {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4: {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                                          static_cast<int>(element.GetComponentCount()),
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE,
                                          static_cast<int>(layout.GetStride()),
                                          reinterpret_cast<const void *>(element.Offset));
                    m_VertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool: {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribIPointer(m_VertexBufferIndex,
                                           static_cast<int>(element.GetComponentCount()),
                                           ShaderDataTypeToOpenGLBaseType(element.Type),
                                           static_cast<int>(layout.GetStride()),
                                           reinterpret_cast<const void *>(element.Offset));
                    m_VertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4: {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++) {
                        glEnableVertexAttribArray(m_VertexBufferIndex);
                        glVertexAttribPointer(m_VertexBufferIndex,
                                              count,
                                              ShaderDataTypeToOpenGLBaseType(element.Type),
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              static_cast<int>(layout.GetStride()),
                                              reinterpret_cast<const void *>(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_VertexBufferIndex, 1);
                        m_VertexBufferIndex++;
                    }
                    break;
                }
                default:
                    EZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Arc<IndexBuffer> &indexBuffer) {
        glBindVertexArray(m_RendererID.get());
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }

    void OpenGLVertexArray::VertexArrayDeleter::operator()(uint32_t ptr) const {
        EZ_CORE_INFO("Deleting VertexArray {0}", ptr);
        glDeleteVertexArrays(1, &ptr);
    }
}
