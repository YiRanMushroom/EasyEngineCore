export module Easy.Core.Engine;

import Easy.Core.Context;
import Easy.Core.Basic;

namespace Easy {
    export class Engine {
    public:
        void Run();

        Arc<EngineContext> GetContext() {
            return m_EngineContext;
        }

    private:
        Engine() = default;

        Arc<EngineContext> m_EngineContext{};

    public:
        static Arc<Engine> FromContext(Arc<EngineContext> context) {
            auto engine = Arc<Engine>(new Engine());
            engine->m_EngineContext = std::move(context);
            engine->m_EngineContext->Init();
            return engine;
        }

        ~Engine() {
            if (m_EngineContext) {
                m_EngineContext->Destroy();
            }
        }
    };
}
