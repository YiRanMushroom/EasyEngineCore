module;

#include <Core/MacroUtils.hpp>

export module Easy.Scripting.JniBind;

export import Easy.Scripting.JniBindBasic;
import Easy.Core.Basic;
import Easy.Core.Util;

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

        export template<typename T>
        auto Cast(auto &&obj) {
            return ClassInfoOf<T>::AsNativeType(std::forward<decltype(obj)>(obj));
        }

        constexpr auto ObjectStaticMethodPtrOf = [](jclass cls, jmethodID methodId, jvalue *args) -> jobject {
            return env->CallStaticObjectMethodA(cls, methodId, args);
        };

        constexpr auto ObjectInstanceMethodPtrOf = [](jobject obj, jmethodID methodId, jvalue *args) -> jobject {
            return env->CallObjectMethodA(obj, methodId, args);
        };

        template<>
        struct ClassInfoOf<int> {
            using ReturnType = int32_t;
            static constexpr char Signature[] = "I";

            static inline int32_t AsNativeType(LocalObject<JTInteger> obj) {
                return Lib::UnBoxOrDefault(obj);
            }

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

        template<>
        struct ClassInfoOf<JBox<int>> {
            using ReturnType = int32_t;
            static constexpr char Signature[] = "Ljava/lang/Integer;";

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> int32_t {
                    LocalObject<JTInteger> obj = LocalObject<JTInteger>{
                        env->CallStaticObjectMethodA(cls, methodId, args)
                    };

                    return ClassInfoOf<int>::AsNativeType(LocalObject<JTInteger>(static_cast<jobject>(obj)));
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject pobj, jmethodID methodId, jvalue *args) -> int32_t {
                    LocalObject<JTInteger> obj = LocalObject<JTInteger>{
                        env->CallObjectMethodA(pobj, methodId, args)
                    };

                    return ClassInfoOf<int>::AsNativeType(LocalObject<JTInteger>(static_cast<jobject>(obj)));
                };
            }
        };

        template<>
        struct ClassInfoOf<float> {
            using ReturnType = float;
            static constexpr char Signature[] = "F";

            static inline auto AsNativeType(LocalObject<JTFloat> obj) {
                return Lib::UnBoxOrDefault(obj);
            }

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> float {
                    return env->CallStaticFloatMethodA(cls, methodId, args);
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> float {
                    return env->CallFloatMethodA(obj, methodId, args);
                };
            }
        };

        template<>
        struct ClassInfoOf<JBox<float>> {
            using ReturnType = float;
            static constexpr char Signature[] = "Ljava/lang/Float;";


            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> float {
                    LocalObject<JTFloat> obj = LocalObject<JTFloat>{
                        env->CallStaticObjectMethodA(cls, methodId, args)
                    };

                    return ClassInfoOf<float>::AsNativeType(LocalObject<JTFloat>(static_cast<jobject>(obj)));
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject pobj, jmethodID methodId, jvalue *args) -> float {
                    LocalObject<JTFloat> obj = LocalObject<JTFloat>{
                        env->CallObjectMethodA(pobj, methodId, args)
                    };

                    return ClassInfoOf<float>::AsNativeType(LocalObject<JTFloat>(static_cast<jobject>(obj)));
                };
            }
        };

        template<>
        struct ClassInfoOf<double> {
            using ReturnType = double;
            static constexpr char Signature[] = "D";

            static inline auto AsNativeType(LocalObject<JTDouble> obj) {
                return Lib::UnBoxOrDefault(obj);
            }

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> double {
                    return env->CallStaticDoubleMethodA(cls, methodId, args);
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> double {
                    return env->CallDoubleMethodA(obj, methodId, args);
                };
            }
        };

        template<>
        struct ClassInfoOf<JBox<double>> {
            using ReturnType = double;
            static constexpr char Signature[] = "Ljava/lang/Double;";


            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> double {
                    LocalObject<JTDouble> obj = LocalObject<JTDouble>{
                        env->CallStaticObjectMethodA(cls, methodId, args)
                    };

                    return ClassInfoOf<double>::AsNativeType(LocalObject<JTDouble>(static_cast<jobject>(obj)));
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject pobj, jmethodID methodId, jvalue *args) -> double {
                    LocalObject<JTDouble> obj = LocalObject<JTDouble>{
                        env->CallObjectMethodA(pobj, methodId, args)
                    };

                    return ClassInfoOf<double>::AsNativeType(LocalObject<JTDouble>(static_cast<jobject>(obj)));
                };
            }
        };

        template<>
        struct ClassInfoOf<uint64_t> {
            using ReturnType = int64_t;
            static constexpr char Signature[] = "J";

            static inline auto AsNativeType(LocalObject<JTLong> obj) {
                return Lib::UnBoxOrDefault(obj);
            }

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> int64_t {
                    return env->CallStaticLongMethodA(cls, methodId, args);
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> int64_t {
                    return env->CallLongMethodA(obj, methodId, args);
                };
            }
        };

        template<>
        struct ClassInfoOf<JBox<uint64_t>> {
            using ReturnType = int64_t;
            static constexpr char Signature[] = "Ljava/lang/Long;";


            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> int64_t {
                    LocalObject<JTLong> obj = LocalObject<JTLong>{
                        env->CallStaticObjectMethodA(cls, methodId, args)
                    };

                    return ClassInfoOf<uint64_t>::AsNativeType(LocalObject<JTLong>(static_cast<jobject>(obj)));
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject pobj, jmethodID methodId, jvalue *args) -> int64_t {
                    LocalObject<JTLong> obj = LocalObject<JTLong>{
                        env->CallObjectMethodA(pobj, methodId, args)
                    };

                    return ClassInfoOf<uint64_t>::AsNativeType(LocalObject<JTLong>(static_cast<jobject>(obj)));
                };
            }
        };

        template<>
        struct ClassInfoOf<short> {
            using ReturnType = int16_t;
            static constexpr char Signature[] = "S";

            static inline auto AsNativeType(LocalObject<JTShort> obj) {
                return Lib::UnBoxOrDefault(obj);
            }

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> int16_t {
                    return env->CallStaticShortMethodA(cls, methodId, args);
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> int16_t {
                    return env->CallShortMethodA(obj, methodId, args);
                };
            }
        };

        template<>
        struct ClassInfoOf<JBox<short>> {
            using ReturnType = int16_t;
            static constexpr char Signature[] = "Ljava/lang/Short;";

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> int16_t {
                    LocalObject<JTShort> obj = LocalObject<JTShort>{
                        env->CallStaticObjectMethodA(cls, methodId, args)
                    };

                    return ClassInfoOf<short>::AsNativeType(LocalObject<JTShort>(static_cast<jobject>(obj)));
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject pobj, jmethodID methodId, jvalue *args) -> int16_t {
                    LocalObject<JTShort> obj = LocalObject<JTShort>{
                        env->CallObjectMethodA(pobj, methodId, args)
                    };

                    return ClassInfoOf<short>::AsNativeType(LocalObject<JTShort>(static_cast<jobject>(obj)));
                };
            }
        };

        template<>
        struct ClassInfoOf<char> {
            using ReturnType = char;
            static constexpr char Signature[] = "C";

            static inline char AsNativeType(LocalObject<JChar> obj) {
                return Lib::UnBoxOrDefault(obj);
            }

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> char {
                    return env->CallStaticCharMethodA(cls, methodId, args);
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> char {
                    return env->CallCharMethodA(obj, methodId, args);
                };
            }
        };

        template<>
        struct ClassInfoOf<JBox<char>> {
            using ReturnType = char;
            static constexpr char Signature[] = "Ljava/lang/Character;";

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> char {
                    LocalObject<JChar> obj = LocalObject<JChar>{
                        env->CallStaticObjectMethodA(cls, methodId, args)
                    };

                    return ClassInfoOf<char>::AsNativeType(LocalObject<JChar>(static_cast<jobject>(obj)));
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject pobj, jmethodID methodId, jvalue *args) -> char {
                    LocalObject<JChar> obj = LocalObject<JChar>{
                        env->CallObjectMethodA(pobj, methodId, args)
                    };

                    return ClassInfoOf<char>::AsNativeType(LocalObject<JChar>(static_cast<jobject>(obj)));
                };
            }
        };

        template<>
        struct ClassInfoOf<bool> {
            using ReturnType = bool;
            static constexpr char Signature[] = "Z";

            static inline bool AsNativeType(LocalObject<JTBoolean> obj) {
                return Lib::UnBoxOrDefault(obj);
            }

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> bool {
                    return env->CallStaticBooleanMethodA(cls, methodId, args);
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> bool {
                    return env->CallBooleanMethodA(obj, methodId, args);
                };
            }
        };

        template<>
        struct ClassInfoOf<JBox<bool>> {
            using ReturnType = bool;
            static constexpr char Signature[] = "Ljava/lang/Boolean;";

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> bool {
                    LocalObject<JTBoolean> obj = LocalObject<JTBoolean>{
                        env->CallStaticObjectMethodA(cls, methodId, args)
                    };

                    return ClassInfoOf<bool>::AsNativeType(LocalObject<JTBoolean>(static_cast<jobject>(obj)));
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject pobj, jmethodID methodId, jvalue *args) -> bool {
                    LocalObject<JTBoolean> obj = LocalObject<JTBoolean>{
                        env->CallObjectMethodA(pobj, methodId, args)
                    };

                    return ClassInfoOf<bool>::AsNativeType(LocalObject<JTBoolean>(static_cast<jobject>(obj)));
                };
            }
        };

        template<>
        struct ClassInfoOf<void> {
            using ReturnType = void;
            static constexpr char Signature[] = "V";

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> void {
                    env->CallStaticVoidMethodA(cls, methodId, args);
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject obj, jmethodID methodId, jvalue *args) -> void {
                    env->CallVoidMethodA(obj, methodId, args);
                };
            }
        };

        template<>
        struct ClassInfoOf<std::string> {
            using ReturnType = std::string;
            static constexpr char Signature[] = "Ljava/lang/String;";


            static std::string AsNativeType(LocalString obj) {
                return {obj.Pin().ToString().data()};
            }

            static std::string AsNativeType(jstring obj) {
                LocalString localString{obj};
                return {localString.Pin().ToString().data()};
            }

            static std::string AsNativeType(const LocalObject<JTString> &obj) {
                return AsNativeType(static_cast<jstring>(static_cast<jobject>(obj)));
            }

            static inline auto *StaticMethodOf() {
                return +[](jclass cls, jmethodID methodId, jvalue *args) -> std::string {
                    return ClassInfoOf<std::string>::AsNativeType(
                        static_cast<jstring>(env->CallStaticObjectMethodA(cls, methodId, args)));
                };
            }

            static inline auto InstanceMethodOf() {
                return +[](jobject pobj, jmethodID methodId, jvalue *args) -> std::string {
                    return ClassInfoOf<std::string>::AsNativeType(
                        static_cast<jstring>(env->CallObjectMethodA(pobj, methodId, args)));
                };
            }
        };

        template<typename>
        struct GetSignatureImpl {
            static_assert(false, "Not a function type");
        };

        export template<typename T>
        constexpr auto GetSignature() {
            return GetSignatureImpl<T>::Get();
        }

        export template<typename Ret, typename... Args>
        struct GetSignatureImpl<Ret(Args...)> {
            static consteval auto Get() {
                return '(' + ((StringLiteral{ClassInfoOf<Args>::Signature}) + ... ) + ')' +
                       StringLiteral{ClassInfoOf<Ret>::Signature};
            }
        };

        template<typename>
        struct NativeFunctionTypeImpl {
            static_assert(false, "Not a function type");
        };

        template<typename Ret, typename... Args>
        struct NativeFunctionTypeImpl<Ret(Args...)> {
            using Type = typename ClassInfoOf<Ret>::ReturnType(typename ClassInfoOf<Args>::ReturnType...);
        };

        export template<typename T>
        using NativeFunctionType = typename NativeFunctionTypeImpl<T>::Type;

        static_assert(GetSignature<void(int)>() == "(I)V"_sl, "Signature Test");
        static_assert(GetSignature<std::string(JBox<double>, char, JBox<int>)>()
                      == "(Ljava/lang/Double;CLjava/lang/Integer;)Ljava/lang/String;"_sl, "Signature Test");

        static_assert(std::is_same_v<NativeFunctionType<std::string(JBox<double>, char, JBox<int>)>,
                          std::string(double, char, int)>, "Signature Test");
    }

    static_assert("Hello"_sl + "World"_sl == "HelloWorld"_sl, "String Literal Test");
    static_assert("Hellow"_sl + 'w' == "Helloww"_sl, "String Literal Test");
    static_assert('w' + "Hello"_sl == "wHello"_sl, "String Literal Test");
}
