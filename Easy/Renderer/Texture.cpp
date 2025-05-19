module;

module Easy.Renderer.Texture;

import <Core/MacroUtils.hu>;
import Easy.Core.Basic;
import Easy.Renderer.RendererAPI;
#ifdef EZ_RENDERER_OPENGL
import Easy.Platform.Impl.OpenGL.Renderer.Texture;
#endif

namespace Easy {
    Arc<Texture2D> Texture2D::Create(const TextureSpecification &specification) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeArc<OpenGLTexture2D>(specification);
#else
                EZ_CORE_ASSERT(false, "OpenGL is not supported!"); return nullptr;
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Arc<Texture2D> Texture2D::Create(const std::string &path) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeArc<OpenGLTexture2D>(path);
#else
                EZ_CORE_ASSERT(false, "OpenGL is not supported!"); return nullptr;
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
