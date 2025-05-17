module;

#include <Core/MacroUtils.hpp>
#include <GLFW/glfw3.h>

module Easy.Renderer.GraphicsContext;

#ifdef EZ_RENDERER_OPENGL
import Easy.Platform.Impl.OpenGL.Renderer.GraphicsContext;
#endif

import Easy.Core.Basic;
import Easy.Core.Log;
import Easy.Renderer.RendererAPI;

namespace Easy {
    Box<GraphicsContext> GraphicsContext::Create(void *window) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeBox<OpenGLContext>(static_cast<GLFWwindow *>(window));
#else
                EZ_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported!"); return nullptr;
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
