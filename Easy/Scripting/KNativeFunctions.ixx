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

            JUnit(Borrowed, jobject) {}
            JUnit(Owned, jobject) {}

            [[nodiscard]] jobject ToJava() const {
                return static_cast<jobject>(StaticRef<StaticDefinition>().Access<"INSTANCE">().Get());
            }

            [[nodiscard]] LocalObject<Definition> GetObject() const {
                return LocalObject<Definition>(ToJava());
            }
        };
    }

    namespace KNativeFunctions {
        export template<size_t N>
        consteval auto GetSimpleClassNameOfArgLength() {
            static_assert(N <= 8, "N must be less than or equal to 8");
            return "kotlin/jvm/functions/Function"_sl + ToStringLiteral<N>();
        }

        static_assert(GetSimpleClassNameOfArgLength<1>() == "kotlin/jvm/functions/Function1"_sl, "Test");
        static_assert(GetSimpleClassNameOfArgLength<2>() == "kotlin/jvm/functions/Function2"_sl, "Test");

        export template<size_t N>
        consteval auto GetSignatureOfArgLength() {
            static_assert(N <= 8, "N must be less than or equal to 8");
            constexpr static StringLiteral javaObjectSig = "Ljava/lang/Object;";
            return '(' + []<size_t... idx>(std::index_sequence<idx...>) {
                return (((void) idx, javaObjectSig) + ...);
            }(std::make_index_sequence<N>()) + ')' + javaObjectSig;
        }

        static_assert(GetSignatureOfArgLength<1>() == "(Ljava/lang/Object;)Ljava/lang/Object;"_sl, "Test");
        static_assert(GetSignatureOfArgLength<2>() == "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;"_sl,
                      "Test");
    }
}
