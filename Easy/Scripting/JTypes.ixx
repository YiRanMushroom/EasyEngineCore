export module Easy.Scripting.JTypes;

import Easy.Core.Basic;
import Easy.Core.Util;
import Easy.Scripting.JniBindBasic;

using namespace jni;

namespace Easy::ScriptingEngine::JTypes {
    template<size_t N>
    consteval auto MakeFullName(StringLiteral<N> SimpleName) {
        return 'L' + SimpleName + ';';
    }

    export class Owned {};

    export class Borrowed {};

    template<const auto& Def>
    class OwnedLocalObjectProvider {
    public:
        constexpr static Class Definition = Def;

        [[nodiscard]] jobject GetRawObject() const {
            return static_cast<jobject>(*m_Object);
        }

        explicit OwnedLocalObjectProvider(jobject obj) : m_Object(
            std::make_shared<GlobalObject<Definition>>(PromoteToGlobal{}, obj)) {}

    private:
        std::shared_ptr<GlobalObject<Definition>> m_Object;
    };

    template<const auto& Def>
    class BorrowedLocalObjectProvider {
    public:
        constexpr static Class Definition = Def;

        [[nodiscard]] jobject GetRawObject() const {
            return static_cast<jobject>(m_Object);
        }

        explicit BorrowedLocalObjectProvider(jobject obj) : m_Object(obj) {}

    private:
        jobject m_Object;
    };

    template<const auto& Def>
    class NullObjectProvider {
    public:
        constexpr static Class Definition = Def;

        [[nodiscard]] jobject GetRawObject() const {
            return static_cast<jobject>(nullptr);
        }

        NullObjectProvider() = default;
    };

    template<const auto& Def>
    class LocalObjectProvider {
    public:
        LocalObjectProvider() : m_ObjectProvider(nullptr) {}

        LocalObjectProvider(Owned, jobject obj) : m_ObjectProvider(OwnedLocalObjectProvider<Def>{obj}) {}

        LocalObjectProvider(Borrowed, jobject obj) : m_ObjectProvider(BorrowedLocalObjectProvider<Def>{obj}) {}

    public:
        [[nodiscard]] jobject GetRawObject() const {
            return std::visit([](auto &&provider) { return provider.GetRawObject(); }, m_ObjectProvider);
        }

        [[nodiscard]] LocalObject<Def> GetObject() const {
            return LocalObject<Def>{GetRawObject()};
        }

    private:
        std::variant<OwnedLocalObjectProvider<Def>, BorrowedLocalObjectProvider<Def>, NullObjectProvider<
            Def>>
        m_ObjectProvider;
    };


    export class JString {
    public:
        constexpr static Class Definition{
            "java/lang/String",
        };

        using JavaType = jobject;

        constexpr static StringLiteral SimpleName = "java/lang/String";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        JString() = default;


        JString(Owned, jobject obj)
            : m_ObjectProvider(Owned{}, obj),
              m_NativeCopy(std::make_shared<std::string>(LocalString{static_cast<jstring>(obj)}.Pin().ToString())) {}

        JString(Borrowed, jobject obj)
            : m_ObjectProvider(Borrowed{}, obj),
              m_NativeCopy(std::make_shared<std::string>(LocalString{static_cast<jstring>(obj)}.Pin().ToString())) {}

        explicit JString(std::string_view str)
            : m_ObjectProvider(Owned{}, static_cast<jobject>(static_cast<jstring>(LocalString{str}))),
              m_NativeCopy(std::make_shared<std::string>(str)) {}

        char operator[](size_t index) const {
            return m_NativeCopy->at(index);
        }

        [[nodiscard]] int Length() const {
            return static_cast<int>(m_NativeCopy->length());
        }

        [[nodiscard]] const char *CStr() const {
            return m_NativeCopy->c_str();
        }

        auto begin() const {
            return m_NativeCopy->begin();
        }

        auto end() const {
            return m_NativeCopy->end();
        }

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.l = ToJava();
            return value;
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::shared_ptr<const std::string> m_NativeCopy{};
    };


    export class Jint {
    public:
        using JavaType = jint;
        constexpr static StringLiteral SimpleName = 'I';
        constexpr static StringLiteral FullName = 'I';

    public:
        Jint() = default;

        Jint(jint value) : m_Value(value) {}

        int Get() const {
            return m_Value;
        }

        [[nodiscard]] jint ToJava() const {
            return m_Value;
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.i = ToJava();
            return value;
        }

    private:
        int m_Value{};
    };

    export class JInteger {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Integer";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

        constexpr static Class Definition{
            SimpleName.Data,
            Constructor{jint{}},
            Method{
                "intValue", Return{jint{}}, Params{}
            }
        };

    public:
        JInteger() = default;

        JInteger(int value)
            : m_ObjectProvider(
                  Owned{}, static_cast<jobject>(LocalObject<Definition>{value})),
              m_Value(value) {}

        JInteger(Owned, jobject obj)
            : m_ObjectProvider(Owned{}, obj),
              m_Value(std::make_optional<int>(m_ObjectProvider.GetObject().Call<"intValue">())) {}

        JInteger(Borrowed, jobject obj)
            : m_ObjectProvider(Borrowed{}, obj),
              m_Value(std::make_optional<int>(m_ObjectProvider.GetObject().Call<"intValue">())) {}

        [[nodiscard]] int GetOrDefault() const {
            return m_Value.value_or(0);
        }

        std::optional<int> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.l = ToJava();
            return value;
        }

    private:
        // std::shared_ptr<ILocalObjectProvider<JInteger>> m_ObjectProvider{};
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::optional<const int> m_Value{};
    };

    export class Jfloat {
    public:
        constexpr static StringLiteral SimpleName = 'F';
        constexpr static StringLiteral FullName = 'F';
        using JavaType = jfloat;

    public:
        Jfloat() = default;

        Jfloat(jfloat value) : m_Value(value) {}

        float Get() const {
            return m_Value;
        }

        [[nodiscard]] jfloat ToJava() const {
            return m_Value;
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.f = ToJava();
            return value;
        }

    private:
        float m_Value{};
    };

    export class JFloat {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Float";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

        static constexpr Class Definition{
            SimpleName.Data,
            Constructor{jfloat{}},
            Method{
                "floatValue", Return{jfloat{}}, Params{}
            }
        };

    public:
        JFloat() = default;

        explicit JFloat(float value)
            : m_ObjectProvider(Owned{}, static_cast<jobject>(LocalObject<Definition>{value})),
              m_Value(value) {}

        explicit JFloat(Owned, jobject obj)
            : m_ObjectProvider(Owned{}, obj),
              m_Value(std::make_optional<float>(m_ObjectProvider.GetObject().Call<"floatValue">())) {}

        explicit JFloat(Borrowed, jobject obj)
            : m_ObjectProvider(Borrowed{}, obj),
              m_Value(std::make_optional<float>(m_ObjectProvider.GetObject().Call<"floatValue">())) {}

        [[nodiscard]] float GetOrDefault() const {
            return m_Value.value_or(0.0f);
        }

        [[nodiscard]] std::optional<float> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.l = ToJava();
            return value;
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::optional<const float> m_Value{};
    };

    export class Jdouble {
        constexpr static StringLiteral SimpleName = 'D';
        constexpr static StringLiteral FullName = 'D';
        using JavaType = jdouble;

    public:
        Jdouble() = default;

        Jdouble(jdouble value) : m_Value(value) {}

        double Get() const {
            return m_Value;
        }

        [[nodiscard]] jdouble ToJava() const {
            return m_Value;
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.d = ToJava();
            return value;
        }

    private:
        double m_Value{};
    };

    export class JDouble {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Double";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

        constexpr static Class Definition{
            SimpleName.Data,
            Constructor{jdouble{}},
            Method{
                "doubleValue", Return{jdouble{}}, Params{}
            }
        };

    public:
        JDouble() = default;

        JDouble(double value)
            : m_ObjectProvider(Owned{}, static_cast<jobject>(LocalObject<Definition>{value})),
              m_Value(value) {}

        JDouble(Owned, jobject obj)
            : m_ObjectProvider(Owned{}, obj),
              m_Value(std::make_optional<double>(m_ObjectProvider.GetObject().Call<"doubleValue">())) {}

        JDouble(Borrowed, jobject obj)
            : m_ObjectProvider(Borrowed{}, obj),
              m_Value(std::make_optional<double>(m_ObjectProvider.GetObject().Call<"doubleValue">())) {}

        [[nodiscard]] double GetOrDefault() const {
            return m_Value.value_or(0.0);
        }

        [[nodiscard]] std::optional<double> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.l = ToJava();
            return value;
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::optional<const double> m_Value{};
    };

    // long
    export class Jlong {
    public:
        constexpr static StringLiteral SimpleName = 'J';
        constexpr static StringLiteral FullName = 'J';
        using JavaType = jlong;

    public:
        Jlong() = default;

        Jlong(jlong value) : m_Value(value) {}

        [[nodiscard]] int64_t Get() const {
            return m_Value;
        }

        [[nodiscard]] jlong ToJava() const {
            return m_Value;
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.j = ToJava();
            return value;
        }

    private:
        int64_t m_Value{};
    };

    export class JLong {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Long";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

        constexpr static Class Definition{
            SimpleName.Data,
            Constructor{jlong{}},
            Method{
                "longValue", Return{jlong{}}, Params{}
            }
        };

    public:
        JLong() = default;

        explicit JLong(int64_t value) : m_ObjectProvider(
                                            Owned{}, static_cast<jobject>(LocalObject<Definition>{value})),
                                        m_Value(value) {}


        explicit JLong(Owned, jobject obj)
            : m_ObjectProvider(Owned{}, obj),
              m_Value(std::make_optional<int64_t>(m_ObjectProvider.GetObject().Call<"longValue">())) {}

        explicit JLong(Borrowed, jobject obj)
            : m_ObjectProvider(Borrowed{}, obj),
              m_Value(std::make_optional<int64_t>(m_ObjectProvider.GetObject().Call<"longValue">())) {}

        [[nodiscard]] int64_t GetOrDefault() const {
            return m_Value.value_or(0);
        }

        [[nodiscard]] std::optional<int64_t> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.l = ToJava();
            return value;
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::optional<const int64_t> m_Value{};
    };

    export class Jboolean {
    public:
        constexpr static StringLiteral SimpleName = 'Z';
        constexpr static StringLiteral FullName = 'Z';
        using JavaType = jboolean;

    public:
        Jboolean() = default;

        Jboolean(jboolean value) : m_Value(value) {}

        [[nodiscard]] bool Get() const {
            return m_Value;
        }

        [[nodiscard]] jboolean ToJava() const {
            return m_Value;
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.z = ToJava();
            return value;
        }

    private:
        bool m_Value{};
    };

    export class JBoolean {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Boolean";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

        constexpr static Class Definition{
            SimpleName.Data,
            Constructor{jboolean{}},
            Method{
                "booleanValue", Return{jboolean{}}, Params{}
            }
        };

    public:
        JBoolean() = default;

        explicit JBoolean(bool value)
            : m_ObjectProvider(Owned{}, static_cast<jobject>(LocalObject<Definition>{value})),
              m_Value(value) {}

        explicit JBoolean(Owned, jobject obj)
            : m_ObjectProvider(Owned{}, obj),
              m_Value(std::make_optional<bool>(m_ObjectProvider.GetObject().Call<"booleanValue">())) {}

        explicit JBoolean(Borrowed, jobject obj)
            : m_ObjectProvider(Borrowed{}, obj),
              m_Value(std::make_optional<bool>(m_ObjectProvider.GetObject().Call<"booleanValue">())) {}

        [[nodiscard]] bool GetOrDefault() const {
            return m_Value.value_or(false);
        }

        [[nodiscard]] std::optional<bool> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

        [[nodiscard]] jvalue ToJvalue() const {
            jvalue value{};
            value.l = ToJava();
            return value;
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::optional<const bool> m_Value{};
    };
}

namespace Easy::ScriptingEngine::MethodResolver {
    template<typename>
    struct ResolveSigExactImpl {
        static_assert(false, "Not a function type");
    };

    template<typename Ret, typename... Args>
    struct ResolveSigExactImpl<Ret(Args...)> {
        static consteval auto Get() {
            return '(' + ((Args::FullName) + ... ) + ')' +
                   Ret::FullName;
        }
    };

    template<typename... Args>
    struct ResolveSigExactImpl<void(Args...)> {
        static consteval auto Get() {
            return '(' + ((Args::FullName) + ... ) + ')' + 'V';
        }
    };

    export template<typename T>
    constexpr auto ResolveSigExact() {
        return ResolveSigExactImpl<T>::Get();
    }

    template<typename Ret>
    struct ResolveSigExactImpl<Ret()> {
        static consteval auto Get() {
            return "()"_sl + Ret::FullName;
        }
    };

    template<>
    struct ResolveSigExactImpl<void()> {
        static consteval auto Get() {
            return "()V"_sl;
        }
    };

    template<typename>
    struct ResolveSigStaticImpl {
        static_assert(false, "Not a function type");
    };

    template<typename Ret, typename... Args>
    struct ResolveSigStaticImpl<Ret(JNIEnv *, jclass, Args...)> {
        static consteval auto Get() {
            return ResolveSigExact<Ret(Args...)>();
        }
    };

    export template<typename T>
    consteval auto ResolveSigStatic() {
        return ResolveSigStaticImpl<T>::Get();
    }

    template<typename>
    struct ResolveSigInstanceImpl {
        static_assert(false, "Not a function type");
    };

    template<typename Ret, typename... Args>
    struct ResolveSigInstanceImpl<Ret(JNIEnv *, Args...)> : ResolveSigExactImpl<Ret(Args...)> {
        static consteval auto Get() {
            return ResolveSigExactImpl<Ret(Args...)>::Get();
        }
    };

    template<typename T>
    consteval auto ResolveSigInstance() {
        return ResolveSigInstanceImpl<T>::Get();
    }

    template<typename>
    struct ResolveJNIFuncTypeStaticImpl {
        static_assert(false, "Not a function type");
    };

    template<typename Ret, typename... RArgs>
    struct ResolveJNIFuncTypeStaticImpl<Ret(JNIEnv *, jclass, RArgs...)> {
        using Type = typename Ret::JavaType(JNIEnv *, jclass, typename RArgs::JavaType...);
    };

    template<typename... RArgs>
    struct ResolveJNIFuncTypeStaticImpl<void(JNIEnv *, jclass, RArgs...)> {
        using Type = void(JNIEnv *, jclass, typename RArgs::JavaType...);
    };

    export template<typename T>
    using ResolveJNIFuncTypeStatic = typename ResolveJNIFuncTypeStaticImpl<T>::Type;

    template<typename>
    struct ResolveJNIFuncTypeInstanceImpl {
        static_assert(false, "Not a function type");
    };

    template<typename Ret, typename... RArgs>
    struct ResolveJNIFuncTypeInstanceImpl<Ret(JNIEnv *, RArgs...)> {
        using Type = typename Ret::JavaType(JNIEnv *, typename RArgs::JavaType...);
    };

    export template<typename T>
    using ResolveJNIFuncTypeInstance = typename ResolveJNIFuncTypeInstanceImpl<T>::Type;

    // Wrap Native Func into JNI Function
    template<auto *, typename>
    struct WrapNativeToJNIStaticFunctionImpl {
        static_assert(false, "Function type mismatch");
    };

    template<auto *nativeFunc, typename Ret, typename... Args>
    struct WrapNativeToJNIStaticFunctionImpl<nativeFunc, Ret(JNIEnv *, jclass, Args...)> {
        static typename Ret::JavaType Call(JNIEnv *env, jclass clazz, typename Args::JavaType... args) {
            auto argTup = std::make_tuple(Args{JTypes::Borrowed{}, args}...);
            return std::apply(nativeFunc, std::tuple_cat(std::make_tuple(env, clazz), std::move(argTup))).ToJava();
        }
    };

    template<auto *nativeFunc, typename... Args>
    struct WrapNativeToJNIStaticFunctionImpl<nativeFunc, void(JNIEnv *, jclass, Args...)> {
        static void Call(JNIEnv *env, jclass clazz, typename Args::JavaType... args) {
            auto argTup = std::make_tuple(Args{JTypes::Borrowed{}, args}...);
            std::apply(nativeFunc, std::tuple_cat(std::make_tuple(env, clazz), std::move(argTup)));
        }
    };

    export template<auto * func>
    consteval auto *WrapNativeToJNIStaticFunction() {
        return WrapNativeToJNIStaticFunctionImpl<func, std::remove_pointer_t<decltype(func)>>::Call;
    }

    template<auto *, typename>
    struct WrapNativeToJNIInstanceFunctionImpl {
        static_assert(false, "Function type mismatch");
    };

    template<auto *nativeFunc, typename Ret, typename... Args>
    struct WrapNativeToJNIInstanceFunctionImpl<nativeFunc, Ret(JNIEnv *, Args...)> {
        static typename Ret::JavaType Call(JNIEnv *env, typename Args::JavaType... args) {
            auto argTup = std::make_tuple(Args{JTypes::Borrowed{}, args}...);
            return std::apply(nativeFunc, std::tuple_cat(std::make_tuple(env), std::move(argTup))).ToJava();
        }
    };

    template<auto *nativeFunc, typename... Args>
    struct WrapNativeToJNIInstanceFunctionImpl<nativeFunc, void(JNIEnv *, Args...)> {
        static void Call(JNIEnv *env, typename Args::JavaType... args) {
            auto argTup = std::make_tuple(Args{JTypes::Borrowed{}, args}...);
            std::apply(nativeFunc, std::tuple_cat(std::make_tuple(env), std::move(argTup)));
        }
    };

    template<auto * func>
    consteval auto *WrapNativeToJNIInstanceFunction() {
        return WrapNativeToJNIInstanceFunctionImpl<func, std::remove_pointer_t<decltype(func)>>::Call;
    }
}
