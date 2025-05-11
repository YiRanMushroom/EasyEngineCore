export module Easy.Scripting.KNativeFunctions;

import Easy.Scripting.JniBind;
import Easy.Scripting.JTypes;
import Easy.Core.Basic;
import Easy.Core.Util;

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

            JUnit(NewRef, jobject) {}

            JUnit(PromoteToGlobal, jobject) {}

            JUnit(AdoptGlobal, jobject) {}

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
    }
}
