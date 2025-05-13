module;

#include <Core/MacroUtils.hpp>
#include "libcuckoo/cuckoohash_map.hh"

export module Easy.Scripting.NativeBuffer;
import Easy.Scripting.JTypes;
import Easy.Scripting.JniBind;
import Easy.Core.Basic;
import Easy.Vendor.libcuckoo;
import Easy.Core.Application;

namespace Easy::ScriptingEngine {
    export class AutoManagedBufferBase {
    public:
        AutoManagedBufferBase() {}

        virtual ~AutoManagedBufferBase() {

        }
    };

    inline libcuckoo::cuckoohash_map<AutoManagedBufferBase *, nullptr_t> g_NativeBuffers = []() {
        libcuckoo::cuckoohash_map<AutoManagedBufferBase *, nullptr_t> buffer;
        buffer.reserve(8000000);
        return std::move(buffer);
    }();
    inline std::atomic_size_t g_CreationCount = 0;

    export void ReleaseAllNativeBufferAfterJVMClose() {
        auto&& locked = g_NativeBuffers.lock_table();
        EZ_CORE_INFO("Releasing {} native buffers", locked.size());
        for (const auto &it : locked) { // NOLINT
            auto buffer = it.first;
            delete buffer;
        }
        locked.clear();
    }

    void NativeReleaseFn(JTypes::Jlong bufferPtr) {
        g_NativeBuffers.erase(reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get()));
        delete reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get());
    }

    void NotifyCreateFunction(JTypes::Jlong bufferPtr) {
        g_NativeBuffers.insert(reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get()), nullptr);
        size_t originalCount = g_CreationCount.fetch_add(1);
        if (originalCount % 10000000 == 500000) {
            g_CreationCount = 0;
            Application::Get().SubmitToMainThread([]() {
                Lib::CallGC();
            });
        }
    }

    // inline std::unordered_set<AutoManagedBufferBase *> g_NativeBuffers;
    // inline std::mutex g_NativeBuffersMutex;
    // inline size_t g_CreationCount = 0;
    //
    // export void ReleaseAllNativeBufferAfterJVMClose() {
    //     std::lock_guard lock(g_NativeBuffersMutex);
    //     EZ_CORE_INFO("Releasing {} native buffers", g_NativeBuffers.size());
    //     for (auto buffer : g_NativeBuffers) {
    //         delete buffer;
    //     }
    //     g_NativeBuffers.clear();
    // }
    //
    // export void NativeReleaseFn(JTypes::Jlong bufferPtr) {
    //     delete reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get());
    //     std::lock_guard lock(g_NativeBuffersMutex);
    //     g_NativeBuffers.erase(reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get()));
    // }
    //
    // export void NotifyCreateFunction(JTypes::Jlong bufferPtr) {
    //     std::lock_guard lock(g_NativeBuffersMutex);
    //     g_NativeBuffers.insert(reinterpret_cast<AutoManagedBufferBase *>(bufferPtr.Get()));
    //     g_CreationCount++;
    //     if (g_CreationCount > 500000) {
    //         Application::Get().SubmitToMainThread([]() {
    //             Lib::CallGC();
    //         });
    //     }
    //     g_CreationCount = 0;
    // }

    export void RegisterNativeBufferRelease() {
        EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<NativeReleaseFn>(
            "com/easy/NativeBuffer$NativeReleaser", "NativeDelete"));
        EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<NotifyCreateFunction>(
            "com/easy/NativeBuffer$NativeReleaser", "NotifyCreate"));
    }
}
