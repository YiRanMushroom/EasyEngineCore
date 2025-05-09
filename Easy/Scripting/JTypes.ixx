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

    export class JString {
    public:
        using JavaType = jstring;

        constexpr static StringLiteral SimpleName = "java/lang/String";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        JString() = default;

        explicit JString(jobject obj) : m_Copy(
            std::make_shared<const std::string>(LocalString{static_cast<jstring>(obj)}.Pin().ToString())) {}

        explicit JString(jstring obj) : m_Copy(
            std::make_shared<const std::string>(LocalString{obj}.Pin().ToString())) {}

        JString(const JString &other) : m_Copy(other.m_Copy) {}

        JString(std::string_view str) : m_Copy(
            std::make_shared<const std::string>(str)) {}

        char operator[](size_t index) const {
            return m_Copy->at(index);
        }

        [[nodiscard]] int Length() const {
            return static_cast<int>(m_Copy->length());
        }

        [[nodiscard]] const char *CStr() const {
            return m_Copy->c_str();
        }

        auto operator<=>(const JString &other) const = default;

        auto begin() const {
            return m_Copy->begin();
        }

        auto end() const {
            return m_Copy->end();
        }

        [[nodiscard]] jstring ToJava() const {
            return static_cast<jstring>(
                LocalString{*m_Copy}
            );
        }

    private:
        std::shared_ptr<const std::string> m_Copy{};
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

    private:
        int m_Value{};
    };

    export class JInteger {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Integer";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

    private:
        constexpr static Class Definition{
            SimpleName.Data,
            Constructor{jint{}},
            Method{
                "intValue", Return{jint{}}, Params{}
            }
        };

    public:
        JInteger() = default;

        JInteger(jobject obj) {
            if (obj != nullptr) {
                m_Value = LocalObject<Definition>{obj}.Call<"intValue">();
            }
        }

        [[nodiscard]] int GetOrDefault() const {
            return m_Value.value_or(0);
        }

        std::optional<int> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_Value ? static_cast<jobject>(LocalObject<Definition>{*m_Value}) : nullptr;
        }

    private:
        std::optional<int> m_Value{};
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

    private:
        float m_Value{};
    };

    export class JFloat {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Float";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

    private:
        static constexpr Class Definition{
            SimpleName.Data,
            Constructor{jfloat{}},
            Method{
                "floatValue", Return{jfloat{}}, Params{}
            }
        };

    public:
        JFloat() = default;

        JFloat(jobject obj) {
            if (obj != nullptr) {
                m_Value = LocalObject<Definition>{obj}.Call<"floatValue">();
            }
        }

        [[nodiscard]] float GetOrDefault() const {
            return m_Value.value_or(0.0f);
        }

        [[nodiscard]] std::optional<float> Get() const {
            return m_Value;
        }

        jobject ToJava() const {
            return m_Value ? static_cast<jobject>(LocalObject<Definition>{*m_Value}) : nullptr;
        }

    private:
        std::optional<float> m_Value{};
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

    private:
        double m_Value{};
    };

    export class JDouble {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Double";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

    private:
        constexpr static Class Definition{
            SimpleName.Data,
            Constructor{jdouble{}},
            Method{
                "doubleValue", Return{jdouble{}}, Params{}
            }
        };

    public:
        JDouble() = default;

        JDouble(jobject obj) {
            if (obj != nullptr) {
                m_Value = LocalObject<Definition>{obj}.Call<"doubleValue">();
            }
        }

        [[nodiscard]] double GetOrDefault() const {
            return m_Value.value_or(0.0);
        }

        [[nodiscard]] std::optional<double> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_Value ? static_cast<jobject>(LocalObject<Definition>{*m_Value}) : nullptr;
        }

    private:
        std::optional<double> m_Value{};
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

    private:
        int64_t m_Value{};
    };

    export class JLong {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Long";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

    private:
        constexpr static Class Definition{
            SimpleName.Data,
            Constructor{jlong{}},
            Method{
                "longValue", Return{jlong{}}, Params{}
            }
        };

    public:
        JLong() = default;

        JLong(jobject obj) {
            if (obj != nullptr) {
                m_Value = LocalObject<Definition>{obj}.Call<"longValue">();
            }
        }

        [[nodiscard]] int64_t GetOrDefault() const {
            return m_Value.value_or(0);
        }

        [[nodiscard]] std::optional<int64_t> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_Value ? static_cast<jobject>(LocalObject<Definition>{*m_Value}) : nullptr;
        }

    private:
        std::optional<int64_t> m_Value{};
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

    private:
        bool m_Value{};
    };

    export class JBoolean {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Boolean";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

    private:
        constexpr static Class Definition{
            SimpleName.Data,
            Constructor{jboolean{}},
            Method{
                "booleanValue", Return{jboolean{}}, Params{}
            }
        };

    public:
        JBoolean() = default;

        JBoolean(jobject obj) {
            if (obj != nullptr) {
                m_Value = LocalObject<Definition>{obj}.Call<"booleanValue">();
            }
        }

        [[nodiscard]] bool GetOrDefault() const {
            return m_Value.value_or(false);
        }

        [[nodiscard]] std::optional<bool> Get() const {
            return m_Value;
        }

        [[nodiscard]] jobject ToJava() const {
            return m_Value ? static_cast<jobject>(LocalObject<Definition>{*m_Value}) : nullptr;
        }

    private:
        std::optional<bool> m_Value{};
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
            auto argTup = std::make_tuple(Args{args}...);
            return std::apply(nativeFunc, std::tuple_cat(std::make_tuple(env, clazz), std::move(argTup))).ToJava();
        }
    };

    template<auto *nativeFunc, typename... Args>
    struct WrapNativeToJNIStaticFunctionImpl<nativeFunc, void(JNIEnv *, jclass, Args...)> {
        static void Call(JNIEnv *env, jclass clazz, typename Args::JavaType... args) {
            auto argTup = std::make_tuple(Args{args}...);
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
            auto argTup = std::make_tuple(Args{args}...);
            return std::apply(nativeFunc, std::tuple_cat(std::make_tuple(env), std::move(argTup))).ToJava();
        }
    };

    template<auto *nativeFunc, typename... Args>
    struct WrapNativeToJNIInstanceFunctionImpl<nativeFunc, void(JNIEnv *, Args...)> {
        static void Call(JNIEnv *env, typename Args::JavaType... args) {
            auto argTup = std::make_tuple(Args{args}...);
            std::apply(nativeFunc, std::tuple_cat(std::make_tuple(env), std::move(argTup)));
        }
    };

    template<auto * func>
    consteval auto *WrapNativeToJNIInstanceFunction() {
        return WrapNativeToJNIInstanceFunctionImpl<func, std::remove_pointer_t<decltype(func)>>::Call;
    }
}
