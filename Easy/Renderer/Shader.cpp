module;

module Easy.Renderer.Shader;

import <Core/MacroUtils.hpp>;
import Easy.Core.Basic;
import Easy.Core.Log;
import Easy.Vendor.glm;

import Easy.Renderer.RendererAPI;

#if defined(EZ_RENDERER_OPENGL)
import Easy.Platform.Impl.OpenGL.Renderer.Shader;
#endif

namespace Easy {
    Arc<Shader> Shader::Create(const std::string &filepath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
#if defined(EZ_RENDERER_OPENGL)
                return MakeArc<OpenGLShader>(filepath);
#else
                EZ_CORE_ASSERT(false, "OpenGL RendererAPI is not supported!");
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Arc<Shader> Shader::Create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: EZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
#ifdef EZ_RENDERER_OPENGL
                return MakeArc<OpenGLShader>(name, vertexSrc, fragmentSrc);
#else
                EZ_CORE_ASSERT(false, "OpenGL RendererAPI is not supported!");
#endif
        }

        EZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    void ShaderLibrary::Add(const std::string &name, const Arc<Shader> &shader) {
        EZ_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Arc<Shader> &shader) {
        const auto &name = shader->GetName();
        Add(name, shader);
    }

    Arc<Shader> ShaderLibrary::Load(const std::string &filepath) {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Arc<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath) {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Arc<Shader> ShaderLibrary::Get(const std::string &name) {
        EZ_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string &name) const {
        return m_Shaders.contains(name);
    }
}
