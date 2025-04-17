export module Easy.Platform.OpenGLEngineContext;

import Easy.Core.Context;

namespace Easy {
    export class OpenGLEngineContext : public EngineContext {
    public:
        virtual void Init() override;

        virtual void Destroy() override;

        virtual void *GetNativeWindow() override {
            return m_Window;
        }

    private:
        GLFWwindow *m_Window{};
    };
}
