module Easy.Scripting.JniBind;

namespace Easy::ScriptingEngine {
    namespace Lib {
        void Init() {
            StaticRef().Call<"Init">();
        }

        void Shutdown() {
            // Do Nothing
        }
    }

    void Shutdown() {
        Lib::Shutdown();
        jvm.reset();
    }
}
