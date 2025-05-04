module;

#include <Core/MacroUtils.hpp>

module Easy.Renderer.Buffer;

import Easy.Core.Basic;

import Easy.Renderer.RendererAPI;

#ifdef EZ_RENDERER_OPENGL
import Easy.Platform.Impl.OpenGL.Renderer.Buffer;
#endif

namespace Easy {
    Arc<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeArc<OpenGLVertexBuffer>(size);
#else
                EZ_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported!"); return nullptr;
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Arc<VertexBuffer> VertexBuffer::Create(const float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeArc<OpenGLVertexBuffer>(vertices, size);
#else
                EZ_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported!"); return nullptr;
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Arc<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeArc<OpenGLIndexBuffer>(indices, size);
#else
                EZ_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported!"); return nullptr;
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}