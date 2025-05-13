module;

#include <Core/MacroUtils.hpp>

export module Easy.Scripting.NativeBuffer;
import Easy.Scripting.JTypes;
import Easy.Scripting.JniBind;
import Easy.Core.Basic;

namespace Easy::ScriptingEngine {
    export class AutoManagedBufferBase {
    public:
        AutoManagedBufferBase() {}

        virtual ~AutoManagedBufferBase() {

        }
    };

    inline std::unordered_set<AutoManagedBufferBase *> g_NativeBuffers;
    inline std::mutex g_NativeBufferMutex;

    export void ReleaseAllNativeBufferAfterJVMClose() {
        std::lock_guard lg(g_NativeBufferMutex);
        EZ_CORE_INFO("{} Native Buffers to be released", g_NativeBuffers.size());
        for (auto *buffer: g_NativeBuffers) {
            delete buffer;
        }
    }

    void NativeReleaseFn(JTypes::Jlong bufferPtr) {
        auto *buffer = reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get());
        {
            std::lock_guard lg(g_NativeBufferMutex);
            g_NativeBuffers.erase(buffer);
        }
        delete buffer;
    }

    void NotifyCreateFunction(JTypes::Jlong bufferPtr) {
        auto *buffer = reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get());
        std::lock_guard lg(g_NativeBufferMutex);
        g_NativeBuffers.insert(buffer);
    }

    export void RegisterNativeBufferRelease() {
        EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<NativeReleaseFn>(
            "com/easy/NativeBuffer$NativeReleaser", "NativeDelete"));
        EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<NotifyCreateFunction>(
            "com/easy/NativeBuffer$NativeReleaser", "NotifyCreate"));
    }
}
