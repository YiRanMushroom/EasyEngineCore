module;

#include <Core/MacroUtils.hpp>

module Easy.Scripting.JniBind;

namespace Easy::ScriptingEngine {
    namespace Lib {
        void NativePrintlnImpl(JNIEnv *, jclass cls, ScriptingEngine::JTypes::
                               JString str) {
            std::cout << str.CStr() << std::endl;
        }

        void Init() {
            StaticRef().Call<"Init">();
            Lib::LibClassObj = MakeBox<GlobalObject<JTClass>>(
                PromoteToGlobal{}, static_cast<jobject>(GetClass("com.easy.Lib"))
            );

            ScriptingEngine::RegisterNativeStaticMethodStatic<
                NativePrintlnImpl>("com/easy/Lib", "NativePrintlnImpl");
        }

        void Shutdown() {
            Lib::LibClassObj.reset();
        }

        LocalObject<JTClass> GetClass(const char *name) {
            std::string newName = name;
            std::replace(newName.begin(), newName.end(), '/', '.');
            return StaticRef().Call<"GetClass">(newName);
        }

        void PrintClassInfo(const LocalObject<JTClass> &classObj) {
            StaticRef().Call<"PrintClassInfo">(classObj);
        }

        LocalObject<JTMethod>
        GetMethodFromClassAndFunctionName(const LocalObject<JTClass> &classObj,
                                          const char *methodName,
                                          const LocalArray<jobject, 1, JTClass> &paraArray) {
            return StaticRef().Call<"GetMethodFromClassAndFunctionName">(
                classObj, methodName, paraArray);
        }

        LocalObject<JTObject>
        CallStaticMethod(const LocalObject<JTMethod> &method,
                         const LocalArray<jobject, 1, JTObject> &paraArray) {
            return StaticRef().Call<"CallStaticMethod">(method, paraArray);
        }
    }

    void Shutdown() {
        Lib::Shutdown();
        jvm.reset();
    }

    bool RegisterNativeMethodDynamicRaw(jclass clazz, const char *methodName, const char *methodSig, void *methodPtr) {
        JNINativeMethod method[1];
        method[0].name = const_cast<char *>(methodName);
        method[0].signature = const_cast<char *>(methodSig);
        method[0].fnPtr = methodPtr;
        return GetEnv()->RegisterNatives(clazz, method, 1) == JNI_OK;
    }


    namespace Util {
        namespace TypeDefinitions {}
    };
}
