module;

import <Core/MacroUtils.hpp>;

module Easy.Renderer.UniformBuffer;

#ifdef EZ_RENDERER_OPENGL
import Easy.Platform.Impl.OpenGL.Renderer.UniformBuffer;
#endif

import Easy.Renderer.RendererAPI;
namespace Easy {
    Arc<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeArc<OpenGLUniformBuffer>(size, binding);
#else
                EZ_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported!");
                return nullptr;
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
