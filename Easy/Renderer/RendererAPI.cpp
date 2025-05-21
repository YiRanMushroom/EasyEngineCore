module;
import <Core/MacroUtils.hpp>;
module Easy.Renderer.RendererAPI;

import Easy.Core.Basic;
import Easy.Core.Log;
import Easy.Core.ApplicationContext;

#ifdef EZ_RENDERER_OPENGL
import Easy.Platform.Impl.OpenGL.Renderer.RendererAPI;
import Easy.Platform.Impl.OpenGL.Renderer.Shader;
#endif

namespace Easy {
    Arc<Shader> RendererAPI::CreateShader(const std::string &name, const std::string &vertexSrc,
                                          const std::string &fragmentSrc) {
        switch (GetAPI()) {
            case RendererAPI::API::None:
                EZ_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
#if defined(EZ_RENDERER_OPENGL)
                return MakeArc<OpenGLShader>(name, vertexSrc, fragmentSrc);
#else
                EZ_CORE_ASSERT(false, "OpenGL RendererAPI is not supported");
#endif
        }
    }

    Arc<Shader> RendererAPI::CreateShader(const std::string &filepath) {
        switch (GetAPI()) {
            case RendererAPI::API::None:
                EZ_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
#if defined(EZ_RENDERER_OPENGL)
                return MakeArc<OpenGLShader>(filepath);
#else
                EZ_CORE_ASSERT(false, "OpenGL RendererAPI is not supported");
#endif
        }
    }

    Box<RendererAPI> RendererAPI::Create() {
        switch (ApplicationContext::ApplicationRendererAPI) {
            case RendererAPI::API::None:
                EZ_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeBox<OpenGLRendererAPI>();
#else
                EZ_CORE_ASSERT(false, "OpenGL RendererAPI is not supported");
#endif
            default:
                EZ_CORE_ASSERT(false, "Unknown RendererAPI");
        }
    }


}
