module;
#include <cstddef>
#include <utility>
#include <tuple>
#include <string>
#include <string_view>
#include <iostream>
#include <memory>
#include <sstream>
#include <algorithm>
#include <mutex>
#include <array>
#include <functional>
import Easy.Core.Basic;
#include <jni_bind_release.h>
// #include "Core/MacroUtils.hpp"
export module Easy.Scripting.JniBind;


export {
    using ::JNIEnv;
    using ::jclass;
    using ::jstring;
    using ::jobject;
    using ::JavaVMInitArgs;
    constexpr auto Jni_Version_1_6 = JNI_VERSION_1_6;
    using ::JavaVMOption;
    constexpr auto Jni_False = JNI_FALSE;
    constexpr auto Jni_True = JNI_TRUE;
    constexpr auto Jni_Ok = JNI_OK;
    using ::JavaVM;
    using ::jint;
    using ::jlong;
    using ::jbyte;
    using ::jboolean;
    using ::jchar;
    using ::jfloat;
    using ::jdouble;
    using ::jshort;
    using ::jobjectArray;
    using ::jclass;
    using ::jmethodID;
    using ::jfieldID;
    using ::jthrowable;
    using ::jvalue;
    using ::JNI_CreateJavaVM;
    using ::JNINativeMethod;
}

export using jni::operator==;
export using jni::operator!=;

export namespace jni {
    using jni::JvmRef;
    using jni::kDefaultJvm;
    using jni::LocalString;
    using jni::GlobalString;
    using jni::LocalObject;
    using jni::GlobalObject;
    using jni::LocalArray;
    using jni::Class;
    using jni::Method;
    using jni::Static;
    using jni::Constructor;
    using jni::Field;
    using jni::Return;
    using jni::Params;
    using jni::Array;
    using jni::StaticRef;
    using jni::PromoteToGlobal;
    using jni::AdoptGlobal;
}

namespace jni {
    // JT+ClassName is a constexpr Class of jni_bind function.
    export constexpr Class JTString{
        "java/lang/String",
        jni::Constructor{},
        jni::Constructor{jstring{}},
    };

    export constexpr Class JTClass{
        "java/lang/Class"
    };

    export constexpr Class JTMethod{
        "java/lang/reflect/Method"
    };

    export constexpr Class JTObject{
        "java/lang/Object",
        jni::Constructor{},
    };
}


namespace Easy {
    using namespace jni;

    namespace ScriptEngine {
        export inline JNIEnv *env;
        export inline JavaVM *pjvm;

        export class ECLib {
        public:
            static constexpr auto JniType = Class{
                "com/easy/Lib",
                Static{
                    Method{"Init", Return{}, Params{}},
                    Method{"GetClass", Return{JTClass}, Params{jstring{}}},
                    Method{"PrintClassInfo", Return{}, Params{JTClass}},
                    Method{
                        "GetMethodFromClassAndFunctionName",
                        Return{JTMethod}, Params{JTClass, jstring{}, Array{JTClass}}
                    },
                    Method{
                        "CallStaticMethod",
                        Return{JTObject}, Params{JTMethod, Array{JTObject}}
                    },
                },
            };

            // inline static constexpr auto StaticRef = jni::StaticRef<JniType>();

            inline static GlobalObject<JTClass> JClassInstance = nullptr;

            void Init();

            void Shutdown();
        };

        export inline Box<jni::JvmRef<jni::kDefaultJvm>> jvm = nullptr;

        export template<size_t N>
        void Init(JavaVMInitArgs args, const JavaVMOption (&option)[N]) {
            args.nOptions = N;
            args.options = const_cast<JavaVMOption *>(option);
            jint rc = JNI_CreateJavaVM(&pjvm, (void **) &env, &args);

            // EZ_CORE_ASSERT(rc == Jni_Ok, "Failed to create Java VM");

            jvm = MakeBox<jni::JvmRef<jni::kDefaultJvm>>(pjvm);
        }
    }
}
