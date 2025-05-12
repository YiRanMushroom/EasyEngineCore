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

        virtual ~AutoManagedBufferBase() {}
    };

    inline std::unordered_set<AutoManagedBufferBase *> g_NativeBuffers;

    export void ReleaseAllNativeBufferAfterJVMClose() {
        EZ_CORE_INFO("{} Native Buffers to be released", g_NativeBuffers.size());
        for (auto *buffer: g_NativeBuffers) {
            delete buffer;
        }
    }

    void NativeReleaseFn(JNIEnv *, jclass, JTypes::Jlong bufferPtr) {
        auto *buffer = reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get());
        g_NativeBuffers.erase(buffer);
        delete buffer;
    }

    void NotifyCreateFunction(JNIEnv *, jclass, JTypes::Jlong bufferPtr) {
        auto *buffer = reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get());
        g_NativeBuffers.insert(buffer);
    }

    export void RegisterNativeBufferRelease() {
        EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<NativeReleaseFn>(
            "com/easy/NativeBuffer$NativeReleaser", "NativeDelete"));
        EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<NotifyCreateFunction>(
            "com/easy/NativeBuffer$NativeReleaser", "NotifyCreate"));
    }
}
