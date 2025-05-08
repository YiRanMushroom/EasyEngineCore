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
        };

        export template<typename T>
        struct JBox {};

        template<>
        struct ClassInfoOf<int> {
            using NativeType = int32_t;
            using JavaType = jint;
            static constexpr char Signature[] = "I";

            static inline NativeType CastToNative(JavaType value) {
                return value;
            }

            static inline JavaType CastToJava(NativeType value) {
                return value;
            }

            static inline NativeType FromJvalue(jvalue value) {
                return value.i;
            }
        };

        template<>
        struct ClassInfoOf<JBox<int>> {
            using NativeType = int32_t;
            using JavaType = jobject;
            static constexpr char Signature[] = "Ljava/lang/Integer;";

            static inline NativeType CastToNative(JavaType obj) {
                return Lib::UnBoxOrDefault(static_cast<LocalObject<JTInteger>>(obj));
            }

            static inline JavaType CastToJava(NativeType value) {
                static constexpr Class IntClass{
                    "java/lang/Integer",
                    Constructor{jint{}}
                };
                LocalObject<IntClass> obj{value};
                return static_cast<jobject>(obj);
            }

            static inline NativeType FromJvalue(jvalue value) {
                return CastToNative(value.l);
            }
        };

        template<>
        struct ClassInfoOf<float> {
            using NativeType = float;
            using JavaType = jfloat;
            static constexpr char Signature[] = "F";

            inline static NativeType CastToNative(JavaType value) {
                return value;
            }

            inline static JavaType CastToJava(NativeType value) {
                return value;
            }

            inline static NativeType FromJvalue(jvalue value) {
                return value.f;
            }
        };

        template<>
        struct ClassInfoOf<JBox<float>> {
            using NativeType = float;
            using JavaType = jobject;
            static constexpr char Signature[] = "Ljava/lang/Float;";

            inline static NativeType CastToNative(JavaType obj) {
                return Lib::UnBoxOrDefault(static_cast<LocalObject<JTFloat>>(obj));
            }

            inline static JavaType CastToJava(NativeType value) {
                static constexpr Class FloatClass{
                    "java/lang/Float",
                    Constructor{jfloat{}}
                };
                LocalObject<FloatClass> obj{value};
                return static_cast<jobject>(obj);
            }

            inline static NativeType FromJvalue(jvalue value) {
                return CastToNative(value.l);
            }
        };

        template<>
        struct ClassInfoOf<double> {
            using NativeType = double;
            using JavaType = jdouble;
            static constexpr char Signature[] = "D";

            static inline NativeType CastToNative(JavaType value) {
                return value;
            }

            static inline JavaType CastToJava(NativeType value) {
                return value;
            }

            static inline NativeType FromJvalue(jvalue value) {
                return value.d;
            }
        };

        template<>
        struct ClassInfoOf<JBox<double>> {
            using NativeType = double;
            using JavaType = jobject;
            static constexpr char Signature[] = "Ljava/lang/Double;";

            inline static NativeType CastToNative(JavaType obj) {
                return Lib::UnBoxOrDefault(static_cast<LocalObject<JTDouble>>(obj));
            }

            inline static JavaType CastToJava(NativeType value) {
                static constexpr Class DoubleClass{
                    "java/lang/Double",
                    Constructor{jdouble{}}
                };
                LocalObject<DoubleClass> obj{value};
                return static_cast<jobject>(obj);
            }

            inline static NativeType FromJvalue(jvalue value) {
                return CastToNative(value.l);
            }
        };

        template<>
        struct ClassInfoOf<int64_t> {
            using NativeType = int64_t;
            using JavaType = jlong;
            static constexpr char Signature[] = "J";

            static inline NativeType CastToNative(JavaType value) {
                return value;
            }

            static inline JavaType CastToJava(NativeType value) {
                return value;
            }

            static inline NativeType FromJvalue(jvalue value) {
                return value.j;
            }
        };

        template<>
        struct ClassInfoOf<JBox<int64_t>> {
            using NativeType = int64_t;
            using JavaType = jobject;
            static constexpr char Signature[] = "Ljava/lang/Long;";

            static inline NativeType CastToNative(JavaType obj) {
                return Lib::UnBoxOrDefault(static_cast<LocalObject<JTLong>>(obj));
            }

            static inline JavaType CastToJava(NativeType value) {
                static constexpr Class LongClass{
                    "java/lang/Long",
                    Constructor{jlong{}}
                };
                LocalObject<LongClass> obj{value};
                return static_cast<jobject>(obj);
            }

            static inline NativeType FromJvalue(jvalue value) {
                return CastToNative(value.l);
            }
        };

        template<>
        struct ClassInfoOf<int16_t> {
            using NativeType = int16_t;
            using JavaType = jshort;
            static constexpr char Signature[] = "S";

            static inline NativeType CastToNative(JavaType value) {
                return value;
            }

            static inline JavaType CastToJava(NativeType value) {
                return value;
            }

            static inline NativeType FromJvalue(jvalue value) {
                return value.s;
            }
        };

        template<>
        struct ClassInfoOf<JBox<int16_t>> {
            using NativeType = int16_t;
            using JavaType = jobject;
            static constexpr char Signature[] = "Ljava/lang/Short;";

            static inline NativeType CastToNative(JavaType obj) {
                return Lib::UnBoxOrDefault(static_cast<LocalObject<JTShort>>(obj));
            }

            static inline JavaType CastToJava(NativeType value) {
                static constexpr Class ShortClass{
                    "java/lang/Short",
                    Constructor{jshort{}}
                };
                LocalObject<ShortClass> obj{value};
                return static_cast<jobject>(obj);
            }

            static inline NativeType FromJvalue(jvalue value) {
                return CastToNative(value.l);
            }
        };

        template<>
        struct ClassInfoOf<char> {
            using NativeType = char;
            using JavaType = jchar;
            static constexpr char Signature[] = "C";

            static inline NativeType CastToNative(JavaType value) {
                return value;
            }

            static inline JavaType CastToJava(NativeType value) {
                return value;
            }

            static inline NativeType FromJvalue(jvalue value) {
                return value.c;
            }
        };

        template<>
        struct ClassInfoOf<JBox<char>> {
            using NativeType = char;
            using JavaType = jobject;
            static constexpr char Signature[] = "Ljava/lang/Character;";

            static inline NativeType CastToNative(JavaType obj) {
                return Lib::UnBoxOrDefault(static_cast<LocalObject<JChar>>(obj));
            }

            static inline JavaType CastToJava(NativeType value) {
                static constexpr Class CharClass{
                    "java/lang/Character",
                    Constructor{jchar{}}
                };
                LocalObject<CharClass> obj{value};
                return static_cast<jobject>(obj);
            }

            static inline NativeType FromJvalue(jvalue value) {
                return CastToNative(value.l);
            }
        };

        template<>
        struct ClassInfoOf<bool> {
            using NativeType = bool;
            using JavaType = jboolean;
            static constexpr char Signature[] = "Z";

            static inline NativeType CastToNative(JavaType value) {
                return value;
            }

            static inline JavaType CastToJava(NativeType value) {
                return value;
            }

            static inline NativeType FromJvalue(jvalue value) {
                return value.z;
            }
        };

        template<>
        struct ClassInfoOf<JBox<bool>> {
            using NativeType = bool;
            using JavaType = jobject;
            static constexpr char Signature[] = "Ljava/lang/Boolean;";

            static inline NativeType CastToNative(JavaType obj) {
                return Lib::UnBoxOrDefault(static_cast<LocalObject<JTBoolean>>(obj));
            }

            static inline JavaType CastToJava(NativeType value) {
                static constexpr Class BooleanClass{
                    "java/lang/Boolean",
                    Constructor{jboolean{}}
                };
                LocalObject<BooleanClass> obj{value};
                return static_cast<jobject>(obj);
            }

            static inline NativeType FromJvalue(jvalue value) {
                return CastToNative(value.l);
            }
        };

        template<>
        struct ClassInfoOf<void> {
            using NativeType = void;
            using JavaType = void;
            static constexpr char Signature[] = "V";

            static inline NativeType CastToNative(JavaType) {}

            static inline JavaType CastToJava(NativeType) {}
        };

        template<>
        struct ClassInfoOf<std::string> {
            using NativeType = std::string;
            using JavaType = jobject;
            static constexpr char Signature[] = "Ljava/lang/String;";

            static inline NativeType CastToNative(JavaType obj) {
                LocalString str{static_cast<jstring>(obj)};
                return str.Pin().ToString().data();
            }

            static inline JavaType CastToJava(NativeType value) {
                LocalString str{value};
                return static_cast<jobject>(static_cast<jstring>(str));
            }

            static inline NativeType FromJvalue(jvalue value) {
                return CastToNative(value.l);
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
        struct StaticNativeFunctionTypeImpl {
            static_assert(false, "Not a function type");
        };

        template<typename Ret, typename... Args>
        struct StaticNativeFunctionTypeImpl<Ret(Args...)> {
            using Type = typename ClassInfoOf<Ret>::NativeType(JNIEnv *, jclass,
                                                               typename ClassInfoOf<Args>::NativeType...);
        };

        export template<typename T>
        using StaticNativeFunctionType = typename StaticNativeFunctionTypeImpl<T>::Type;

        template<typename, typename>
        struct InstanceNativeFunctionTypeImpl {
            static_assert(false, "Second is not a function type");
        };

        template<typename InstanceType, typename Ret, typename... Args>
        struct InstanceNativeFunctionTypeImpl<InstanceType, Ret(Args...)> {
            using Type = typename ClassInfoOf<Ret>::NativeType(JNIEnv *, typename ClassInfoOf<InstanceType>::NativeType,
                                                               typename ClassInfoOf<Args>::NativeType...);
        };

        export template<typename InstanceType, typename T>
        using InstanceNativeFunctionType = typename InstanceNativeFunctionTypeImpl<InstanceType, T>::Type;

        static_assert(GetSignature<void(int)>() == "(I)V"_sl, "Signature Test");
        static_assert(GetSignature<std::string(JBox<double>, char, JBox<int>)>()
                      == "(Ljava/lang/Double;CLjava/lang/Integer;)Ljava/lang/String;"_sl, "Signature Test");

        static_assert(std::is_same_v<StaticNativeFunctionType<std::string(JBox<double>, char, JBox<int>)>,
                          std::string(JNIEnv *, jclass, double, char, int)>, "Signature Test");

        static_assert(
            std::is_same_v<InstanceNativeFunctionType<std::string, std::string(JBox<double>, char, JBox<int>)>,
                std::string(JNIEnv *, std::string, double, char, int)>, "Signature Test");

        template<typename>
        struct JNIStaticFunctionTypeImpl {
            static_assert(false, "Not a function type");
        };

        template<typename Ret, typename... Args>
        struct JNIStaticFunctionTypeImpl<Ret(Args...)> {
            using Type = typename ClassInfoOf<Ret>::JavaType(JNIEnv *, jclass, jvalue *);
        };

        template<typename>
        struct JNIInstanceFunctionTypeImpl {
            static_assert(false, "Second is not a function type");
        };

        template<typename Ret, typename... Args>
        struct JNIInstanceFunctionTypeImpl<Ret(Args...)> {
            using Type =
            typename ClassInfoOf<Ret>::JavaType(JNIEnv *, jobject, jvalue *);
        };

        // export template<typename T>
        // using JNIStaticFunctionType = typename JNIStaticFunctionTypeImpl<T>::Type;
        //
        // export template<typename Func>
        // using JNIInstanceFunctionType = typename JNIInstanceFunctionTypeImpl<Func>::Type;

        template<auto *>
        struct WrapNativeToJNIStaticFunctionImpl {
            static_assert(false, "Not a function ptr");
        };

        template<typename Ret, typename... Args, Ret(*func)(Args...)>
        struct WrapNativeToJNIStaticFunctionImpl<func> {
            constexpr static auto Wrap()
                -> decltype(auto) {
                constexpr static auto *ret = +[](JniEnv *env, jclass clazz, jvalue *args) -> typename Ret::JavaType {
                    constexpr size_t argc = sizeof...(Args);

                    return [=]<size_t... idx>(std::index_sequence<idx...>) {
                        return func(env, clazz,
                                    ClassInfoOf<Args>::FromJvalue(args[idx])...);
                    }(std::make_index_sequence<argc>());
                };
                return ret;
            }
        };

        export template<auto *func>
        constexpr auto WrapNativeToJNIStaticFunction() {
            return WrapNativeToJNIStaticFunctionImpl<func>::Wrap();
        }

        template<typename T, auto *, typename Fn>
        struct WrapNativeToJNIInstanceFunctionImpl {
            static_assert(false, "Not a function ptr");
        };

        template<typename InfoT, typename Ret, typename... Args, auto *func, typename Ignore1, typename Ignore2>
        struct WrapNativeToJNIInstanceFunctionImpl<InfoT, func, Ret(Ignore1, Ignore2, Args...)> {
            static_assert(std::is_same_v<Ignore2, typename InfoT::NativeType>,
                          "The first argument must be the instance type");

            constexpr static auto Wrap() {
                constexpr size_t argc = sizeof...(Args);

                return [=]<size_t... idx>(std::index_sequence<idx...>) {
                    return +[](JniEnv *env, jobject obj, jvalue *args) -> decltype(auto) {
                        return func(env, InfoT::CastToNative(obj),
                                    ClassInfoOf<Args>::FromJvalue(args[idx])...);
                    };
                }(std::make_index_sequence<argc>());
            }
        };

        export template<typename T, auto *fn>
        constexpr auto WrapNativeToJNIInstanceFunction() {
            return WrapNativeToJNIInstanceFunctionImpl<ClassInfoOf<T>, fn, std::remove_pointer_t<std::remove_cv_t<
                decltype(fn
                )>>>::Wrap();
        }

        namespace Test {
            constexpr void (*TestFnPtr)(JniEnv *, std::string, std::string, int) = nullptr;

            constexpr auto TestFn = WrapNativeToJNIInstanceFunction<std::string, TestFnPtr>();

            static_assert(
                std::is_same_v<decltype(TestFn), void(*const)(JniEnv *, jobject, jvalue *)>,
                "Test Function Type"
            );
        }
    }

    static_assert("Hello"_sl + "World"_sl == "HelloWorld"_sl, "String Literal Test");
    static_assert("Hellow"_sl + 'w' == "Helloww"_sl, "String Literal Test");
    static_assert('w' + "Hello"_sl == "wHello"_sl, "String Literal Test");
}
