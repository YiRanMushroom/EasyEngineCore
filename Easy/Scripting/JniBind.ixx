module;

#include <Core/MacroUtils.hpp>

export module Easy.Scripting.JniBind;

export import Easy.Scripting.JniBindBasic;
import Easy.Core.Basic;

using namespace jni;

namespace Easy::ScriptingEngine {
    export inline JNIEnv *env;
    export inline JavaVM *pjvm;

    namespace Lib {
        export constexpr auto JniType = Class{
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
                // Method{
                //     "UnBoxOrDefault",
                //     Return{jint{}}, Params{JTInteger}
                // },
                // Method{
                //     "UnBoxOrDefault",
                //     Return{jfloat{}}, Params{JTFloat}
                // },
                // Method{
                //     "UnBoxOrDefault",
                //     Return{jdouble{}}, Params{JTDouble}
                // },
                // Method{
                //     "UnBoxOrDefault",
                //     Return{jlong{}}, Params{JTLong}
                // },
                // Method{
                //     "UnBoxOrDefault",
                //     Return{jshort{}}, Params{JTShort}
                // },
                // Method{
                //     "UnBoxOrDefault",
                //     Return{jbyte{}}, Params{JTByte}
                // },
                // Method{
                //     "UnBoxOrDefault",
                //     Return{jchar{}}, Params{JChar}
                // },
                // Method{
                //     "UnBoxOrDefault",
                //     Return{jboolean{}}, Params{JTBoolean}
                // },
                Method{
                    "UnBoxOrDefault",
                    Overload{
                        Return{jint{}}, Params{JTInteger}
                    },
                    Overload{
                        Return{jfloat{}}, Params{JTFloat}
                    },
                    Overload{
                        Return{jdouble{}}, Params{JTDouble}
                    },
                    Overload{
                        Return{jlong{}}, Params{JTLong}
                    },
                    Overload{
                        Return{jshort{}}, Params{JTShort}
                    },
                    Overload{
                        Return{jbyte{}}, Params{JTByte}
                    },
                    Overload{
                        Return{jchar{}}, Params{JChar}
                    },
                    Overload{
                        Return{jboolean{}}, Params{JTBoolean}
                    }
                }
            },
        };

        export constexpr const auto &StaticRef() {
            constexpr static const auto &ret = jni::StaticRef<JniType>();
            return ret;
        }

        export void Init();

        export void Shutdown();

        export LocalObject<JTClass> GetClass(const char *name);

        export Box<decltype(GlobalObject<JTClass>{
            PromoteToGlobal{}, (jobject) Lib::GetClass("com.easy.Lib")
        })> LibClassObj = nullptr;

        export void PrintClassInfo(const LocalObject<JTClass> &classObj);

        export LocalObject<JTMethod> GetMethodFromClassAndFunctionName(
            const LocalObject<JTClass> &classObj,
            const char *methodName,
            const LocalArray<jobject, 1, JTClass> &paraArray
        );

        export LocalObject<JTObject> CallStaticMethod(
            const LocalObject<JTMethod> &method,
            const LocalArray<jobject, 1, JTObject> &paraArray
        );

        export template<typename ArrayType, typename... Objs>
        auto CreateObjectArray(Objs &&... objs) {
            ArrayType ret{sizeof...(objs)};
            std::tuple tuple = std::forward_as_tuple(std::forward<Objs>(objs)...);

            [&]<size_t... idx>(std::index_sequence<idx...>) {
                (ret.Set(idx, static_cast<jobject>(std::get<idx>(tuple))), ...);
            }(std::make_index_sequence<sizeof...(objs)>());

            return std::move(ret);
        }

        export inline int32_t UnBoxOrDefault(const LocalObject<JTInteger> &obj) {
            return StaticRef().Call<"UnBoxOrDefault">(obj);
        }

        export inline float UnBoxOrDefault(const LocalObject<JTFloat> &obj) {
            return StaticRef().Call<"UnBoxOrDefault">(obj);
        }

        export inline double UnBoxOrDefault(const LocalObject<JTDouble> &obj) {
            return StaticRef().Call<"UnBoxOrDefault">(obj);
        }

        export inline int64_t UnBoxOrDefault(const LocalObject<JTLong> &obj) {
            return StaticRef().Call<"UnBoxOrDefault">(obj);
        }

        export inline int16_t UnBoxOrDefault(const LocalObject<JTShort> &obj) {
            return StaticRef().Call<"UnBoxOrDefault">(obj);
        }

        export inline int8_t UnBoxOrDefault(const LocalObject<JTByte> &obj) {
            return StaticRef().Call<"UnBoxOrDefault">(obj);
        }

        export inline char UnBoxOrDefault(const LocalObject<JChar> &obj) {
            return StaticRef().Call<"UnBoxOrDefault">(obj);
        }

        export inline bool UnBoxOrDefault(const LocalObject<JTBoolean> &obj) {
            return StaticRef().Call<"UnBoxOrDefault">(obj);
        }
    }

    export inline Box<jni::JvmRef<jni::kDefaultJvm>> jvm = nullptr;

    export template<size_t N>
    void Init(JavaVMInitArgs args, const JavaVMOption (&option)[N]) {
        args.nOptions = N;
        args.options = const_cast<JavaVMOption *>(option);
        jint rc = JNI_CreateJavaVM(&pjvm, (void **) &env, &args);

        EZ_CORE_ASSERT(rc == JNI_OK, "Failed to create Java VM");

        jvm = MakeBox<jni::JvmRef<jni::kDefaultJvm>>(pjvm);

        Lib::Init();
    }

    export void Shutdown();

    namespace Util::TypeDefinitions {
        export template<typename T>
        struct ClassInfoOf {
            static_assert(false, "Type needs to be specialized");

            // ReturnType
            // ParameterType is Always jvalue
            // Method to call when static
            // Method to call when instance
        };

        export template<typename T>
        struct JBox {};

        //GetStaticMethodPtr
        constexpr auto ObjectStaticMethodPtrOf = [](jclass cls, jmethodID methodId, jvalue *args) -> jobject {
            return env->CallStaticObjectMethodA(cls, methodId, args);
        };

        constexpr auto ObjectInstanceMethodPtrOf = [](jobject obj, jmethodID methodId, jvalue *args) -> jobject {
            return env->CallObjectMethodA(obj, methodId, args);
        };

        export template<>
        struct ClassInfoOf<int> {
            using ReturnType = int32_t;
            static constexpr char Signature[] = "I";

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> int32_t {
                    return env->CallStaticIntMethodA(cls, methodId, args);
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> int32_t {
                    return env->CallIntMethodA(obj, methodId, args);
                };
            }
        };

        export template<>
        struct ClassInfoOf<JBox<int>> {
            using ReturnType = int32_t;
            static constexpr char Signature[] = "Ljava.lang.Integer;";

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> int32_t {
                    jobject obj = env->CallStaticObjectMethodA(cls, methodId, args);
                    if (obj == nullptr) {
                        return 0;
                    }
                    return 0;
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> int32_t {
                    return env->CallIntMethodA(obj, methodId, args);
                };
            }
        };
    }
}
