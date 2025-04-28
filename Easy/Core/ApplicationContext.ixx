module;

export module Easy.Core.ApplicationContext;

import Easy.Renderer.RendererAPI;

namespace Easy::ApplicationContext {
    RendererAPI::API s_ApplicationRendererAPI = RendererAPI::API::OpenGL;

    export RendererAPI::API GetApplicationRendererAPI() {
        return s_ApplicationRendererAPI;
    }

    export void SetApplicationRendererAPI(RendererAPI::API api) {
        s_ApplicationRendererAPI = api;
    }
}
