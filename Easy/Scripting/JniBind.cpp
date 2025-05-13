module;

#include <Core/MacroUtils.hpp>

module Easy.Scripting.JniBind;

import Easy.Scripting.NativeBuffer;
import Easy.Scripting.KNativeFunctions;
import Easy.Scripting.KNativeArrays;
import Easy.ImGui.ImGui;

namespace Easy::ScriptingEngine {
    namespace Lib {
        void NativePrintlnImpl(ScriptingEngine::JTypes::
                               JString str) {
            std::cout << str.Get() << std::endl;
        }

        void RegisterImGuiNativeFunctions();

        void Init() {
            StaticRef().Call<"Init">();
            Lib::LibClassObj = MakeBox<GlobalObject<JTClass>>(
                PromoteToGlobal{}, static_cast<jobject>(GetClass("com.easy.Lib"))
            );

            ScriptingEngine::RegisterNativeStaticMethodStatic<
                NativePrintlnImpl>("com/easy/Lib", "NativePrintlnImpl");

            RegisterImGuiNativeFunctions();
            RegisterNativeBufferRelease();
            ScriptingEngine::KNativeFunctions::RegisterAllNativeInvokeFunction();
            KNativeWString::Init();
            KNativeString::Init();
        }

        void RegisterImGuiNativeFunctions() {
            EZ_CORE_ASSERT(
                ScriptingEngine::RegisterNativeStaticMethodStatic<+[](JTypes::JString jstr) {
                ImGui::Begin(jstr.Get().c_str());
                }>("com/easy/ImGui", "Begin"));
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<+[]() {
                ImGui::End();
                }>("com/easy/ImGui", "End"));
            EZ_CORE_ASSERT(
                ScriptingEngine::RegisterNativeStaticMethodStatic<+[](JTypes::JString jstr) {
                ImGui::Text("%s", jstr.Get().c_str());
                }>("com/easy/ImGui", "Text"));

            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<
                +[](JTypes::JString label, JTypes::JObjRef<JTypes::JFloat> value_ref, JTypes::Jfloat
                    min, JTypes::Jfloat max, JTypes::JString format, JTypes::Jint flags) -> JTypes::Jboolean {
                float value = value_ref.Get().Get().value();
                bool ret = ImGui::SliderFloat(label.Get().c_str(), &value, min.Get(), max.Get(),
                    format.Get().c_str(), flags.Get());
                value_ref.Set(JTypes::JFloat{value});
                return ret;
                }
                >("com/easy/ImGui", "SliderFloat"));

            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<
                +[](JTypes::JString label, JTypes::JObjRef<JTypes::JString> value_ref, JTypes::Jint
                    size, JTypes::Jint flags) -> JTypes::Jboolean {
                std::string value = value_ref.Get().Get();
                value.reserve(size.Get());
                bool ret = ImGui::InputText(label.Get().c_str(), value.data(), size.Get(), flags.Get());
                value_ref.Set(JTypes::JString{value});
                return ret;
                }
                >("com/easy/ImGui", "InputText"));

            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<
                +[](JTypes::JString label, JTypes::JObjRef<JTypes::JBoolean> value_ref) -> JTypes::
                Jboolean {
                bool value = value_ref.Get().Get().value();
                bool ret = ImGui::Checkbox(label.Get().c_str(), &value);
                value_ref.Set(JTypes::JBoolean{value});
                return ret;
                }
                >("com/easy/ImGui", "CheckBox"));
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

        void CallGC() {
            constexpr static Class SystemDef{
                "java/lang/System",
                Static{
                    Method{"gc", Return{}, Params{}}
                }
            };

            jni::StaticRef<SystemDef>().Call<"gc">();
        }
    }


    void Shutdown() {
        Lib::Shutdown();
        Lib::CallGC();
        jvm.reset();
        pjvm->DestroyJavaVM();
        ReleaseAllNativeBufferAfterJVMClose();
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
