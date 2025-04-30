module;

#include "Core/MacroUtils.hpp"

module Easy.Renderer.FrameBuffer;

import Easy.Core.Basic;
import Easy.Renderer.RendererAPI;

#ifdef EZ_RENDERER_OPENGL
import Easy.Platform.Impl.OpenGL.Renderer.FrameBuffer;
#endif

namespace Easy {
    Arc<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeArc<OpenGLFramebuffer>(spec);
#else
				EZ_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported!"); return nullptr;
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
