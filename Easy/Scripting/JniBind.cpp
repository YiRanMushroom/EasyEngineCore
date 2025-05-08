module;

module Easy.Scripting.JniBind;

namespace Easy::ScriptingEngine {
    namespace Lib {
        void Init() {
            StaticRef().Call<"Init">();
            Lib::LibClassObj = MakeBox<GlobalObject<JTClass>>(
                PromoteToGlobal{}, static_cast<jobject>(GetClass("com.easy.Lib"))
            );
        }

        void Shutdown() {
            Lib::LibClassObj.reset();
        }

        LocalObject<JTClass> GetClass(const char *name) {
            std::string className = name;
            std::replace(className.begin(), className.end(), '/', '.');
            return StaticRef().Call<"GetClass">(name);
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

    namespace Util {
        namespace TypeDefinitions {

        }
    };
}
