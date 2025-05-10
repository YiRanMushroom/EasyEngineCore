export module Easy.Scripting.JTypes;

import Easy.Core.Basic;
import Easy.Core.Util;
import Easy.Scripting.JniBindBasic;

using namespace jni;

namespace Easy::ScriptingEngine::JTypes {
    export template<size_t N>
    consteval auto MakeFullName(StringLiteral<N> SimpleName) {
        return 'L' + SimpleName + ';';
    }

    export class Owned {};

    export class Borrowed {};

    template<const auto &Def>
    class OwnedLocalObjectProvider {
    public:
        constexpr static const auto &Definition = Def;

        [[nodiscard]] jobject GetRawObject() const {
            return static_cast<jobject>(*m_Object);
        }

        LocalObject<Definition> GetObject() const {
            return LocalObject<Definition>(GetRawObject());
        }

        explicit OwnedLocalObjectProvider(jobject obj) : m_Object(
            std::make_shared<GlobalObject<Definition>>(PromoteToGlobal{}, obj)) {}

    private:
        std::shared_ptr<GlobalObject<Definition>> m_Object;
    };

    template<const auto &Def>
    class BorrowedLocalObjectProvider {
    public:
        constexpr static const auto &Definition = Def;

        [[nodiscard]] jobject GetRawObject() const {
            return static_cast<jobject>(m_Object);
        }

        LocalObject<Definition> GetObject() const {
            return LocalObject<Definition>(GetRawObject());
        }

        explicit BorrowedLocalObjectProvider(jobject obj) : m_Object(obj) {}

    private:
        jobject m_Object;
    };

    template<const auto &Def>
    class NullObjectProvider {
    public:
        constexpr static const auto &Definition = Def;

        [[nodiscard]] jobject GetRawObject() const {
            return static_cast<jobject>(nullptr);
        }

        LocalObject<Definition> GetObject() const {
            return LocalObject<Definition>(GetRawObject());
        }

        NullObjectProvider() = default;
    };

    template<const auto &Def>
    class LocalObjectProvider {
    public:
        LocalObjectProvider() : m_ObjectProvider(NullObjectProvider<
            Def>{}) {}

        LocalObjectProvider(Owned, jobject obj) : m_ObjectProvider(OwnedLocalObjectProvider<Def>{obj}) {}

        LocalObjectProvider(Borrowed, jobject obj) : m_ObjectProvider(BorrowedLocalObjectProvider<Def>{obj}) {}

    public:
        [[nodiscard]] jobject GetRawObject() const {
            return std::visit([](auto &&provider) { return provider.GetRawObject(); }, m_ObjectProvider);
        }

        [[nodiscard]] LocalObject<Def> GetObject() const {
            return std::visit([](auto &&provider) { return provider.GetObject(); }, m_ObjectProvider);
        }

    private:
        std::variant<OwnedLocalObjectProvider<Def>, BorrowedLocalObjectProvider<Def>, NullObjectProvider<
            Def>>
        m_ObjectProvider;
    };

    export jvalue CastToJvalue(const auto &) = delete;

    export jvalue CastToJvalue(jint value) {
        jvalue jv{};
        jv.i = value;
        return jv;
    }

    export jvalue CastToJvalue(jfloat value) {
        jvalue jv{};
        jv.f = value;
        return jv;
    }

    export jvalue CastToJvalue(jdouble value) {
        jvalue jv{};
        jv.d = value;
        return jv;
    }

    export jvalue CastToJvalue(jlong value) {
        jvalue jv{};
        jv.j = value;
        return jv;
    }

    export jvalue CastToJvalue(jshort value) {
        jvalue jv{};
        jv.s = value;
        return jv;
    }

    export jvalue CastToJvalue(jbyte value) {
        jvalue jv{};
        jv.b = value;
        return jv;
    }

    export jvalue CastToJvalue(jchar value) {
        jvalue jv{};
        jv.c = value;
        return jv;
    }

    export jvalue CastToJvalue(jboolean value) {
        jvalue jv{};
        jv.z = value;
        return jv;
    }

    export jvalue CastToJvalue(jobject value) {
        jvalue jv{};
        jv.l = value;
        return jv;
    }

    export struct InjectJObject {
        constexpr static jobject (JNIEnv::*CallStaticMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticObjectMethodA;

        constexpr static jobject (JNIEnv::*CallInstanceMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticObjectMethodA;

        using JavaType = jobject;
    };

    export class JString : public InjectJObject {
    public:
        constexpr static Class Definition{
            "java/lang/String",
        };

        constexpr static StringLiteral SimpleName = "java/lang/String";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        constexpr static jobject (JNIEnv::*CallInstanceMethodA)(jobject, jmethodID, const jvalue *) = &
                JNIEnv::CallObjectMethodA;

        constexpr static jobject (JNIEnv::*CallStaticMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticObjectMethodA;

        JString() = default;

        JString(Owned, jobject obj)
            : m_ObjectProvider(Owned{}, obj),
              m_NativeCopy(
                  std::make_shared<std::string>(LocalString{m_ObjectProvider.GetRawObject()}.Pin().ToString())) {}

        JString(Borrowed, jobject obj)
            : m_ObjectProvider(Borrowed{}, obj),
              m_NativeCopy(
                  std::make_shared<std::string>(LocalString{m_ObjectProvider.GetRawObject()}.Pin().ToString())) {}

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

        [[nodiscard]] const std::string &Get() const {
            return *m_NativeCopy;
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

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::shared_ptr<const std::string> m_NativeCopy{};
    };


    export class Jint {
    public:
        using JavaType = jint;
        constexpr static StringLiteral SimpleName = 'I';
        constexpr static StringLiteral FullName = 'I';

        constexpr static jint (JNIEnv::*CallStaticMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticIntMethodA;

        constexpr static jint (JNIEnv::*CallInstanceMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticIntMethodA;

    public:
        Jint() = default;

        Jint(jint value) : m_Value(value) {}

        Jint(Borrowed, jint value) : m_Value(value) {}
        Jint(Owned, jint value) : m_Value(value) {}

        int Get() const {
            return m_Value;
        }

        [[nodiscard]] jint ToJava() const {
            return m_Value;
        }

    private:
        int m_Value{};
    };

    export class JInteger : public InjectJObject {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Integer";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        constexpr static Class Definition{
            SimpleName.Data,
            Method{
                "intValue", Return{jint{}}, Params{}
            }
        };

        constexpr static Class StaticDefinition{
            SimpleName.Data,
            Static{
                Method{"valueOf", Return{Definition}, Params{jint{}}}
            }
        };

    public:
        JInteger() = default;

        JInteger(int value)
            : m_ObjectProvider(
                  Owned{}, static_cast<jobject>(StaticRef<StaticDefinition>().Call<"valueOf">(jint{value}))),
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

        constexpr static jfloat (JNIEnv::*CallStaticMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticFloatMethodA;

        constexpr static jfloat (JNIEnv::*CallInstanceMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticFloatMethodA;

    public:
        Jfloat() = default;

        Jfloat(jfloat value) : m_Value(value) {}
        Jfloat(Borrowed, jfloat value) : m_Value(value) {}
        Jfloat(Owned, jfloat value) : m_Value(value) {}

        float Get() const {
            return m_Value;
        }

        [[nodiscard]] jfloat ToJava() const {
            return m_Value;
        }

    private:
        float m_Value{};
    };

    export class JFloat : public InjectJObject {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Float";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        static constexpr Class Definition{
            SimpleName.Data,
            Method{
                "floatValue", Return{jfloat{}}, Params{}
            }
        };

        static constexpr Class StaticDefinition{
            SimpleName.Data,
            Static{
                Method{"valueOf", Return{Definition}, Params{jfloat{}}}
            }
        };

    public:
        JFloat() = default;

        explicit JFloat(float value)
            : m_ObjectProvider(
                  Owned{}, static_cast<jobject>(StaticRef<StaticDefinition>().Call<"valueOf">(jfloat{value}))),
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

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::optional<const float> m_Value{};
    };

    export class Jdouble {
    public:
        constexpr static StringLiteral SimpleName = 'D';
        constexpr static StringLiteral FullName = 'D';

        using JavaType = jdouble;

        constexpr static jdouble (JNIEnv::*CallStaticMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticDoubleMethodA;

        constexpr static jdouble (JNIEnv::*CallInstanceMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticDoubleMethodA;

    public:
        Jdouble() = default;

        Jdouble(jdouble value) : m_Value(value) {}
        Jdouble(Borrowed, jdouble value) : m_Value(value) {}
        Jdouble(Owned, jdouble value) : m_Value(value) {}


        double Get() const {
            return m_Value;
        }

        [[nodiscard]] jdouble ToJava() const {
            return m_Value;
        }

    private:
        double m_Value{};
    };

    export class JDouble : public InjectJObject {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Double";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        using JavaType = jobject;

        constexpr static Class Definition{
            SimpleName.Data,
            Method{
                "doubleValue", Return{jdouble{}}, Params{}
            }
        };

        constexpr static Class StaticDefinition{
            SimpleName.Data,
            Static{
                Method{"valueOf", Return{Definition}, Params{jdouble{}}}
            }
        };

    public:
        JDouble() = default;

        JDouble(double value)
            : m_ObjectProvider(
                  Owned{}, static_cast<jobject>(StaticRef<StaticDefinition>().Call<"valueOf">(jdouble{value}))),
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

        constexpr static jlong (JNIEnv::*CallStaticMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticLongMethodA;

        constexpr static jlong (JNIEnv::*CallInstanceMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticLongMethodA;

    public:
        Jlong() = default;

        Jlong(jlong value) : m_Value(value) {}
        Jlong(Borrowed, jlong value) : m_Value(value) {}
        Jlong(Owned, jlong value) : m_Value(value) {}

        [[nodiscard]] int64_t Get() const {
            return m_Value;
        }

        [[nodiscard]] jlong ToJava() const {
            return m_Value;
        }

    private:
        int64_t m_Value{};
    };

    export class JLong : public InjectJObject {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Long";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        constexpr static Class Definition{
            SimpleName.Data,
            Method{
                "longValue", Return{jlong{}}, Params{}
            }
        };

        constexpr static Class StaticDefinition{
            SimpleName.Data,
            Static{
                Method{"valueOf", Return{Definition}, Params{jlong{}}}
            }
        };

    public:
        JLong() = default;

        explicit JLong(int64_t value) : m_ObjectProvider(
                                            Owned{}, static_cast<jobject>(
                                                StaticRef<StaticDefinition>().Call<"valueOf">(jlong{value}))),
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

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::optional<const int64_t> m_Value{};
    };

    export class Jboolean {
    public:
        constexpr static StringLiteral SimpleName = 'Z';
        constexpr static StringLiteral FullName = 'Z';

        using JavaType = jboolean;

        constexpr static jboolean (JNIEnv::*CallStaticMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticBooleanMethodA;

        constexpr static jboolean (JNIEnv::*CallInstanceMethodA)(jclass, jmethodID, const jvalue *) = &
                JNIEnv::CallStaticBooleanMethodA;

    public:
        Jboolean() = default;

        Jboolean(jboolean value) : m_Value(value) {}
        Jboolean(Borrowed, jboolean value) : m_Value(value) {}
        Jboolean(Owned, jboolean value) : m_Value(value) {}

        [[nodiscard]] bool Get() const {
            return m_Value;
        }

        [[nodiscard]] jboolean ToJava() const {
            return m_Value;
        }

    private:
        bool m_Value{};
    };

    export class JBoolean : public InjectJObject {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Boolean";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        constexpr static Class Definition{
            SimpleName.Data,
            Method{
                "booleanValue", Return{jboolean{}}, Params{}
            }
        };

        constexpr static Class StaticDefinition{
            SimpleName.Data,
            Static{
                Method{"valueOf", Return{Definition}, Params{jboolean{}}}
            }
        };

    public:
        JBoolean() = default;

        explicit JBoolean(bool value)
            : m_ObjectProvider(Owned{}, static_cast<jobject>(
                                   StaticRef<StaticDefinition>().Call<"valueOf">(jboolean{
                                       static_cast<jboolean>(value)
                                   }))),
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

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
        std::optional<const bool> m_Value{};
    };

    export class JObject : public InjectJObject {
    public:
        constexpr static StringLiteral SimpleName = "java/lang/Object";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        constexpr static Class Definition{
            SimpleName.Data
        };

        JObject() = default;

        JObject(Borrowed, jobject obj) : m_ObjectProvider(Borrowed{}, obj) {}
        JObject(Owned, jobject obj) : m_ObjectProvider(Owned{}, obj) {}

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

        [[nodiscard]] LocalObject<Definition> GetObject() const {
            return m_ObjectProvider.GetObject();
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
    };


    export template<typename InnerType>
    class JObjRef : public InjectJObject {
        static_assert(std::is_same_v<typename InnerType::JavaType, jobject>,
                      "InnerType must be a jobject type");

    public:
        constexpr static StringLiteral SimpleName = "com/easy/ObjRef";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);

        constexpr static Class Definition{
            "com/easy/ObjRef",
            Constructor{},
            Constructor{jobject{}},
            Method{"Get", Return{JObject::Definition}, Params{}},
            Method{"Set", Return{}, Params{JObject::Definition}},
        };

        JObjRef() = default;

        JObjRef(Borrowed, jobject obj) : m_ObjectProvider(Borrowed{}, obj) {}

        JObjRef(Owned, jobject obj) : m_ObjectProvider(Owned{}, obj) {}

        JObjRef(const InnerType &obj) : m_ObjectProvider(Owned{}, obj.ToJava()) {}

        jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

        InnerType Get() const {
            LocalObject<Definition> localObj = m_ObjectProvider.GetObject();
            return InnerType{Borrowed{}, static_cast<jobject>(localObj.template Call<"Get">())};
        }

        void Set(const InnerType &obj) {
            m_ObjectProvider.GetObject().template Call<"Set">(obj.ToJava());
        }

        jobject GetRawObject() const {
            return m_ObjectProvider.GetRawObject();
        }

        LocalObject<Definition> GetObject() const {
            return m_ObjectProvider.GetObject();
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
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
        static_assert(false, "Function Parameter Type Mismatch");
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
