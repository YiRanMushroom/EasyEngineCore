module;

#include <stdlib.h>

#include "Core/MacroUtils.hpp"

export module Easy.Scripting.KNativeFunctions;

import Easy.Scripting.JniBind;
import Easy.Scripting.JTypes;
import Easy.Core.Basic;
import Easy.Core.Util;
import Easy.Scripting.NativeBuffer;

using namespace jni;

namespace Easy::ScriptingEngine {
    namespace JTypes {
        export class JUnit : public JTypes::InjectJObject {
        public:
            constexpr static StringLiteral SimpleName = "kotlin/Unit";
            constexpr static StringLiteral FullName = ScriptingEngine::JTypes::MakeFullName(SimpleName);

            constexpr static Class Definition{
                FullName.Data,
            };

            constexpr static Class StaticDefinition{
                FullName.Data,
                Static{
                    Field{"INSTANCE", Definition},
                }
            };

            JUnit() = default;

            template<typename T>
            explicit JUnit(T, jobject obj) {}

            [[nodiscard]] jobject ToJava() const {
                return static_cast<jobject>(StaticRef<StaticDefinition>().Access<"INSTANCE">().Get());
            }

            [[nodiscard]] LocalObject<Definition> GetObject() const {
                return LocalObject<Definition>(ToJava());
            }

        public:
            static JUnit GetInstance() {
                return JUnit{};
            }
        };
    }

    namespace KNativeFunctions {
        export constexpr auto MaxArgCount = 8;

        export template<size_t N>
        consteval auto GetSimpleFunctionInterfaceNameOfArgLength() {
            static_assert(N <= MaxArgCount, "N must be less than or equal to MaxArgCount");
            return "kotlin/jvm/functions/Function"_sl + ToStringLiteral<N>();
        }

        export template<size_t N>
        consteval auto GetSimpleNativeFunctionClassNameOfArgLength() {
            static_assert(N <= MaxArgCount, "N must be less than or equal to MaxArgCount");
            return "com/easy/NativeFunction"_sl + ToStringLiteral<N>();
        }

        export template<size_t N>
        consteval auto GetParamsOfArgLength() {
            static_assert(N <= MaxArgCount, "N must be less than or equal to MaxArgCount");
            return []<size_t... idx>(std::index_sequence<idx...>) {
                return Params{((void) idx, JTypes::JObject::Definition)...};
            }(std::make_index_sequence<N>());
        }

        static_assert(GetSimpleFunctionInterfaceNameOfArgLength<1>() == "kotlin/jvm/functions/Function1"_sl, "Test");
        static_assert(GetSimpleFunctionInterfaceNameOfArgLength<2>() == "kotlin/jvm/functions/Function2"_sl, "Test");

        export template<size_t N>
        consteval auto GetObjectFunctionTypeSignatureOfArgLength() {
            static_assert(N <= MaxArgCount, "N must be less than or equal to MaxArgCount");
            constexpr static StringLiteral javaObjectSig = "Ljava/lang/Object;";
            return '(' + []<size_t... idx>(std::index_sequence<idx...>) {
                return (((void) idx, javaObjectSig) + ...);
            }(std::make_index_sequence<N>()) + ')' + javaObjectSig;
        }

        export template<>
        consteval auto GetObjectFunctionTypeSignatureOfArgLength<0>() {
            return "()Ljava/lang/Object;"_sl;
        }

        static_assert(GetObjectFunctionTypeSignatureOfArgLength<1>() == "(Ljava/lang/Object;)Ljava/lang/Object;"_sl,
                      "Test");
        static_assert(
            GetObjectFunctionTypeSignatureOfArgLength<2>() ==
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;"_sl,
            "Test");

        template<size_t N>
        struct MakeImplFunctionTypeImpl {
        private:
            template<typename>
            struct ImplType {
                static_assert(false, "Incorrect specialization");
            };

            template<size_t... idx>
            struct ImplType<std::index_sequence<idx...>> {
                using Type = std::remove_pointer_t<jobject((std::remove_cvref_t<decltype((void) idx, jobject{})>...))>;
            };

        public:
            using Type =
            typename ImplType<std::make_index_sequence<N>>::Type;
        };

        export template<size_t N>
        using MakeImplFunctionType = typename MakeImplFunctionTypeImpl<N>::Type;

        template<typename>
        class KNativeFunctionInterfaceBase {
            static_assert(false, "Incorrect specialization");
        };

        using namespace JTypes;

        template<typename Ret, typename... Args>
            requires std::is_same_v<Ret, jobject> && (std::is_same_v<jobject, Args> && ...)
        class KNativeFunctionInterfaceBase<Ret(Args...)> {
        private:
            constexpr static size_t ArgCount = sizeof...(Args);
            static_assert(ArgCount <= MaxArgCount, "N must be less than or equal to MaxArgCount");

        public:
            constexpr static StringLiteral SimpleName =
                    GetSimpleFunctionInterfaceNameOfArgLength<ArgCount>();
            constexpr static StringLiteral FullName =
                    GetSimpleFunctionInterfaceNameOfArgLength<ArgCount>();

            constexpr static Class Definition{
                SimpleName.Data,
                Method{"invoke", Return{JObject::Definition}, GetParamsOfArgLength<ArgCount>()}
            };

        public:
            KNativeFunctionInterfaceBase() = default;

            template<typename T>
            KNativeFunctionInterfaceBase(T, jobject obj) : m_ObjectProvider{
                T{}, obj
            } {}

        public:
            jobject ToJava() const {
                return m_ObjectProvider.GetRawObject();
            }

        protected:
            ScriptingEngine::JTypes::LocalObjectProvider<Definition> m_ObjectProvider{};
        };

        export template<size_t N>
        class KNativeFunctionInterface : public KNativeFunctionInterfaceBase<MakeImplFunctionType<N>> {
        public:
            using Base = KNativeFunctionInterfaceBase<MakeImplFunctionType<N>>;

            KNativeFunctionInterface() = default;

            template<typename T>
            KNativeFunctionInterface(T, jobject obj) : Base{
                T{}, obj
            } {}
        };

        export template<typename>
        class SpecializedKNativeFunctionInterface {
            static_assert(false, "Incorrect specialization");
        };

        export template<typename Ret, typename... Args>
            requires std::is_same_v<jobject, typename Ret::JavaType> &&
                     (std::is_same_v<jobject, typename Args::JavaType> && ...)
        class SpecializedKNativeFunctionInterface<Ret(Args...)> : public KNativeFunctionInterface<sizeof...(Args)> {
        public:
            using Base = KNativeFunctionInterface<sizeof...(Args)>;
            constexpr static const auto &Definition = KNativeFunctionInterface<sizeof...(Args)>::Definition;

            SpecializedKNativeFunctionInterface() = default;

            template<typename T>
            SpecializedKNativeFunctionInterface(T, jobject obj) : Base{
                T{}, obj
            } {}

        private:
            struct RetFnType {
            public:
                Ret operator()(Args... args) {
                    return Ret{
                        PromoteToGlobal{},
                        static_cast<jobject>(this->m_LocalObject.template Call<"invoke">(args.ToJava()...))
                    };
                }

                RetFnType(jobject obj) : m_LocalObject(obj) {}

            private:
                LocalObject<Definition> m_LocalObject;
            };

        public:
            [[nodiscard]] std::optional<RetFnType> Get() const {
                return this->m_ObjectProvider
                           ? std::make_optional<RetFnType>(this->m_ObjectProvider.GetRawObject())
                           : std::optional<RetFnType>{};
            }

            Ret operator()(Args... args) {
                return Get().value()(args...);
            }
        };

        export template<typename... Args>
            requires (std::is_same_v<jobject, typename Args::JavaType> && ...)
        class SpecializedKNativeFunctionInterface<void(Args...)> : public KNativeFunctionInterface<sizeof...(Args)> {
        public:
            using Base = KNativeFunctionInterface<sizeof...(Args)>;
            constexpr static const auto &Definition = KNativeFunctionInterface<sizeof...(Args)>::Definition;

            SpecializedKNativeFunctionInterface() = default;

            template<typename T>
            SpecializedKNativeFunctionInterface(T, jobject obj) : Base{
                T{}, obj
            } {}

        private:
            struct RetFnType {
            public:
                void operator()(Args... args) {
                    this->m_LocalObject.template Call<"invoke">(args.ToJava()...);
                }

                RetFnType(jobject obj) : m_LocalObject(obj) {}

            private:
                LocalObject<Definition> m_LocalObject;
            };

        public:
            [[nodiscard]] std::optional<RetFnType> Get() const {
                return this->m_ObjectProvider.NotNull()
                           ? std::make_optional<RetFnType>(this->m_ObjectProvider.GetRawObject())
                           : std::optional<RetFnType>{};
            }

            void operator()(Args... args) {
                Get().value()(args...);
            }
        };

        template<size_t N>
        class KNativeFunctionImplSized : ScriptingEngine::AutoManagedBufferBase {
            static_assert(N <= KNativeFunctions::MaxArgCount, "N must be less than or equal to MaxArgCount");

        public:
            constexpr static StringLiteral SimpleName = "com/easy/NativeFunction"_sl + ToStringLiteral<N>();
            constexpr static StringLiteral FullName = ScriptingEngine::JTypes::MakeFullName(SimpleName);

            constexpr static Class Definition{
                SimpleName.Data,
                Constructor{jlong{}},
                Method{
                    "CastToInterface", Return{KNativeFunctions::KNativeFunctionInterface<N>::Definition},
                    Params{}
                },
                Method{
                    "invoke", Return{JObject::Definition}, GetParamsOfArgLength<N>()
                },
                Method{
                    "GetNativeAddress", Return{jlong{}}, Params{}
                }
            };

            template<typename>
            struct ImplType {
                static_assert(false, "Incorrect specialization");
            };

            template<size_t... idx>
            struct ImplType<std::index_sequence<idx...>> {
                using Type = std::remove_pointer_t<jni::LocalObject<JObject::Definition>((
                    std::remove_cvref_t<decltype((void) idx, jobject{})>...))>;
            };

        public:
            using FunctionType = typename ImplType<std::make_index_sequence<N>>::Type;
            using StandardFunctionType = std::function<FunctionType>;

            explicit KNativeFunctionImplSized(StandardFunctionType function) : m_Function(std::move(function)) {}

            template<typename... Args>
            LocalObject<JObject::Definition> Invoke(Args... args) {
                static_assert((std::is_same_v<jobject, Args> && ...), "All arguments must be jobject");
                auto result = m_Function(args...);
                return result;
            }

            StandardFunctionType &GetFunction() {
                return m_Function;
            }

        protected:
            StandardFunctionType m_Function;
        };

        template<typename>
        class KNativeFunctionImpl {
            static_assert(false, "Incorrect specialization");
        };

        template<typename Ret, typename... Args>
            requires std::is_same_v<jobject, typename Ret::JavaType> &&
                     (std::is_same_v<jobject, typename Args::JavaType> && ...)
        class KNativeFunctionImpl<Ret(Args...)> : public KNativeFunctionImplSized<sizeof...(Args)> {
        public:
            using Base = KNativeFunctionImplSized<sizeof...(Args)>;

        public:
            virtual ~KNativeFunctionImpl() override = default;

            KNativeFunctionImpl() = default;

            explicit KNativeFunctionImpl(auto &&fn) : Base{
                typename Base::StandardFunctionType(
                    [stored = std::forward<decltype(fn)>(fn)]<typename... Objs>(
                Objs... objects) -> LocalObject<JObject::Definition> {
                        static_assert(( std::is_same_v<Objs, jobject> && ...), "All arguments must be jobject");
                        auto res = stored(Args{AdoptLocal{}, objects}...);
                        return LocalObject<JObject::Definition>{NewRef{}, res.ToJava()};
                    })
            } {}
        };

        export template<typename>
        class KNativeFunction {
            static_assert(false, "Incorrect specialization");
        };

        export template<typename Ret, typename... Args>
            requires std::is_same_v<jobject, typename Ret::JavaType> &&
                     (std::is_same_v<jobject, typename Args::JavaType> && ...)
        class KNativeFunction<Ret(Args...)> {
        public:
            constexpr static StringLiteral SimpleName =
                    "com/easy/NativeFunction"_sl + ToStringLiteral<sizeof...(Args)>();
            constexpr static StringLiteral FullName = ScriptingEngine::JTypes::MakeFullName(SimpleName);

            constexpr static const auto &Definition = KNativeFunctionImplSized<sizeof...(Args)>::Definition;

        public:
            KNativeFunction(std::unique_ptr<KNativeFunctionImplSized<sizeof...(Args)>> fn) {
                auto local = LocalObject<Definition>{reinterpret_cast<jlong>(fn.release())};
                m_ObjectProvider = {
                    AdoptLocal{}, local.Release()
                };
            }

            template<typename T>
            explicit KNativeFunction(T &&fn) : KNativeFunction{
                static_cast<std::unique_ptr<KNativeFunctionImplSized<sizeof...(Args)>>>(std::make_unique<
                    KNativeFunctionImpl<Ret
                        (Args...)>>(std::forward<T>(fn)))
            } {}

            KNativeFunction(const KNativeFunction &) = default;

            KNativeFunction(KNativeFunction &&) = default;

            template<typename T>
            KNativeFunction(T, jobject obj) : m_ObjectProvider{
                T{}, obj
            } {}

            jobject ToJava() const {
                return m_ObjectProvider.GetRawObject();
            }

            [[nodiscard]] LocalObject<Definition> Get() const {
                return m_ObjectProvider.GetObject();
            }

            Ret operator()(Args... args) {
                auto localRef = Get().template Call<"invoke">(args.ToJava()...);
                Ret result{AdoptLocal{}, localRef.Release()};
                return result;
            }

            SpecializedKNativeFunctionInterface<Ret(Args...)> CastToInterface() {
                return {AdoptLocal{}, Get().template Call<"CastToInterface">().Release()};
            }

        private:
            LocalObjectProvider<Definition> m_ObjectProvider;
        };

        // template<size_t N>
        // jobject NativeInvokeFunction(JNIEnv *, jobject func, jobject arg1) {
        //     LocalObject<KNativeFunctionImplSized<N>::Definition> function{AdoptLocal{}, func};
        //
        //     auto nativeFunction = reinterpret_cast<KNativeFunctionImplSized<N> *>(function.template Call<
        //         "GetNativeAddress">());
        //
        //     return nativeFunction->Invoke(arg1).Release();
        // }

        namespace Impl {
            template<typename>
            struct NativeInvokeFunctionImpl {
                static_assert(false, "Incorrect specialization");
            };

            template<size_t... idx>
            struct NativeInvokeFunctionImpl<std::index_sequence<idx...>> {
                static jobject NativeInvokeFunction(
                    JNIEnv *env, jobject func, decltype((void)idx, jobject{})... args) {
                    LocalObject<KNativeFunctionImplSized<sizeof...(idx)>::Definition> function{AdoptLocal{}, func};

                    auto nativeFunction = reinterpret_cast<KNativeFunctionImplSized<sizeof...(idx)> *>(function.template Call<
                        "GetNativeAddress">());

                    return nativeFunction->Invoke(args...).Release();
                }
            };
        }

        template<size_t N>
        void RegisterNativeInvokeFunctionFor() {
            EZ_CORE_ASSERT(RegisterNativeMethodDynamicRaw(
                static_cast<jclass>(static_cast<jobject>(Lib::GetClass(KNativeFunctionImplSized<N>::SimpleName.Data))),
                "NativeInvoke",
                GetObjectFunctionTypeSignatureOfArgLength<N>().Data,
                (void *)&Impl::NativeInvokeFunctionImpl<std::make_index_sequence<N>>::NativeInvokeFunction));
        }

        export void RegisterAllNativeInvokeFunction() {
            constexpr size_t maxArgCount = KNativeFunctions::MaxArgCount;
            []<size_t ... idx>(std::index_sequence<idx...>) {
                (RegisterNativeInvokeFunctionFor<idx>(), ...);
            }(std::make_index_sequence<maxArgCount + 1>{});
        }
    }
}
