//
// Created by Yiran on 2025-04-17.
//

export module Easy.Core.Context;

namespace Easy {
    export class EngineContext {
    public:
        virtual ~EngineContext() = default;

        virtual void Init() = 0;

        virtual void Destroy() = 0;

        virtual void *GetNativeWindow() = 0;

        static EngineContext *GetCurrentContext() {
            return s_CurrentContext;
        }

        inline static EngineContext *s_CurrentContext = nullptr;

        static void SetCurrentContext(EngineContext *context) {
            s_CurrentContext = context;
        }
    };
}
