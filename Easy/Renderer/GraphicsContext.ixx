export module Easy.Renderer.GraphicsContext;

import Easy.Core.Basic;

namespace Easy {
    export class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;

        virtual void SwapBuffers() = 0;

        static Box<GraphicsContext> Create(void *window);
    };
}
