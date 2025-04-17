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
        virtual void* GetNativeWindow() = 0;
    };
}
