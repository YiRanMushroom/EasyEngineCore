module;

#include <Core/MacroUtils.hpp>

export module Easy.Scripting.JniBind;

export import Easy.Scripting.JniBindBasic;
import Easy.Core.Basic;
import Easy.Core.Util;
import Easy.Scripting.JTypes;

using namespace jni;

namespace Easy::ScriptingEngine {
    export inline JNIEnv *env;
    export inline JavaVM *pjvm;

    export inline JNIEnv *GetEnv() { return env; }

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

    export void Init() {
        EZ_ASSERT(create_directory("./Java/lib"));
        EZ_ASSERT(create_directory("./Java/mods"));

        std::vector<std::string> jars{};

        auto fs1 = std::filesystem::current_path() / "Java" / "lib";

        for (const auto &entry: std::filesystem::directory_iterator(fs1)) {
            if (entry.path().extension() == ".jar") {
                EZ_CORE_INFO("Found jar: {0}", entry.path().string());
                jars.push_back(entry.path().string());
            }
        }

        auto fs2 = std::filesystem::current_path() / "Java" / "mods";

        for (const auto &entry: std::filesystem::directory_iterator(fs2)) {
            if (entry.path().extension() == ".jar") {
                EZ_CORE_INFO("Found jar: {0}", entry.path().string());
                jars.push_back(entry.path().string());
            }
        }

        std::stringstream ss{};
        for (auto it = jars.begin(); it != jars.end(); ++it) {
            ss << *it;
            if (it + 1 != jars.end()) {
                ss << ":";
            }
        }

        std::string allJarStringArgs = std::format("-Djava.class.path={}", ss.str());

        JavaVMOption options[1];
        options[0].optionString = allJarStringArgs.data();

        ScriptingEngine::Init({.version = JNI_VERSION_1_6},
                              options);
    }

    export void Shutdown();

    export bool RegisterNativeMethodDynamicRaw(jclass clazz, const char *methodName, const char *methodSig,
                                               void *methodPtr);

    export template<auto * func>
    bool RegisterNativeStaticMethodStatic(jclass clazz, const char *methodName) {
        using namespace ScriptingEngine::MethodResolver;
        constexpr auto sig = ResolveSigStatic<std::remove_pointer_t<decltype(func)>>();
        return ScriptingEngine::RegisterNativeMethodDynamicRaw(
            static_cast<jclass>(static_cast<jobject>(clazz)),
            methodName,
            sig.Data,
            reinterpret_cast<void *>(WrapNativeToJNIStaticFunction<func>())
        );
    }

    export template<auto * func>
    bool RegisterNativeStaticMethodStatic(const char *className, const char *methodName) {
        using namespace ScriptingEngine::MethodResolver;
        auto clazz = Lib::GetClass(className);
        constexpr auto sig = ResolveSigStatic<std::remove_pointer_t<decltype(func)>>();
        return ScriptingEngine::RegisterNativeMethodDynamicRaw(
            static_cast<jclass>(static_cast<jobject>(clazz)),
            methodName,
            sig.Data,
            reinterpret_cast<void *>(WrapNativeToJNIStaticFunction<func>())
        );
    }

    static_assert("Hello"_sl + "World"_sl == "HelloWorld"_sl, "String Literal Test");
    static_assert("Hellow"_sl + 'w' == "Helloww"_sl, "String Literal Test");
    static_assert('w' + "Hello"_sl == "wHello"_sl, "String Literal Test");

    export template<const auto & Def>
    class JavaGlobalArc {
    public:
        constexpr static const auto &Definition = Def;

        JavaGlobalArc() = default;

        JavaGlobalArc(jobject obj) : m_Arc(obj
                                               ? std::make_shared<GlobalObject<Definition>>(PromoteToGlobal{}, obj)
                                               : nullptr) {}

        JavaGlobalArc(const LocalObject<Definition> &obj) : JavaGlobalArc(static_cast<jobject>(obj)) {}

        void Set(jobject obj) {
            *this = JavaGlobalArc(obj);
        }

        void Set(const LocalObject<Definition> &obj) {
            *this = JavaGlobalArc(static_cast<jobject>(obj));
        }

        template<typename T>
        void SetFrom(const T &obj) {
            Set(static_cast<jobject>(obj));
        }

        explicit operator bool() const {
            return m_Arc != nullptr;
        }

        bool NotNull() const {
            return static_cast<bool>(*this);
        }

        bool IsNull() const {
            return !static_cast<bool>(*this);
        }

        explicit operator jobject() const {
            return static_cast<jobject>(*m_Arc);
        }

        explicit operator LocalObject<Definition>() const {
            return LocalObject<Definition>(static_cast<jobject>(*m_Arc));
        }

        GlobalObject<Definition> *operator->() const {
            return m_Arc.get();
        }

        [[nodiscard]] jobject GetRawObject() const {
            return static_cast<jobject>(*m_Arc);
        }

        template<typename T>
        [[nodiscard]] T GetObjectAs() const {
            return static_cast<T>(GetRawObject());
        }

        LocalObject<Definition> Borrow() const {
            return LocalObject<Definition>(GetRawObject());
        }

        LocalObject<Definition> GetObject() const {
            return LocalObject<Definition>(GetRawObject());
        }

    private:
        std::shared_ptr<GlobalObject<Definition>> m_Arc{nullptr};
    };

    export template<typename>
    class JConstructor {
        static_assert(false, "JConstructor is not specialized");
    };

    // constructor hold a methodId
    export template<typename ClassType, typename... Args>
    class JConstructor<ClassType(Args...)> {
    public:
        JConstructor() = default;

        JConstructor(jmethodID id) : id(id) {}

        JConstructor(const char *className) {
            auto clazz = Lib::GetClass(className);
            id = ScriptingEngine::GetEnv()->GetMethodID(
                static_cast<jclass>(static_cast<jobject>(clazz)),
                "<init>", GetSignature().Data
            );
            EZ_CORE_ASSERT(id != nullptr, "Constructor not found");
        }

        JConstructor(jclass clazz) {
            id = ScriptingEngine::GetEnv()->GetMethodID(
                static_cast<jclass>(static_cast<jobject>(clazz)),
                "<init>", GetSignature().Data
            );
            EZ_CORE_ASSERT(id != nullptr, "Constructor not found");
        }

        void Init(jclass clazz) {
            *this = JConstructor(clazz);
        }

        void Init(const char *className) {
            *this = JConstructor(className);
        }

        jobject Invoke(JNIEnv *env, jclass cls, Args... args) {
            EZ_CORE_ASSERT(id != nullptr, "Constructor not found");

            std::array<jvalue, sizeof...(Args)> jniArgs{
                JTypes::CastToJvalue(args.ToJava())...
            };

            return env->NewObjectA(
                cls, id,
                jniArgs.data());
        }

        consteval static auto GetSignature() {
            return ScriptingEngine::MethodResolver::ResolveSigExact<void(Args...)>();
        }

    private:
        jmethodID id = nullptr;
    };

    export template<typename>
    class JInstanceMethod {
        static_assert(false, "JConstructor is not specialized");
    };

    export template<typename Ret, typename Self, typename... Args>
    class JInstanceMethod<Ret(Self, Args...)> {
    public:
        JInstanceMethod() = default;

        JInstanceMethod(jmethodID id) : id(id) {}

        JInstanceMethod(const char *className, const char *methodName) {
            auto clazz = Lib::GetClass(className);
            id = ScriptingEngine::GetEnv()->GetMethodID(
                static_cast<jclass>(static_cast<jobject>(clazz)),
                "methodName", GetSignature().Data
            );
            EZ_CORE_ASSERT(id != nullptr, "Constructor not found");
        }

        JInstanceMethod(jclass clazz, const char *methodName) {
            id = ScriptingEngine::GetEnv()->GetMethodID(
                static_cast<jclass>(static_cast<jobject>(clazz)),
                methodName, GetSignature().Data
            );
            EZ_CORE_ASSERT(id != nullptr, "Constructor not found");
        }

        void Init(jclass clazz, const char *methodName) {
            *this = JInstanceMethod(clazz, methodName);
        }

        void Init(const char *className, const char *methodName) {
            *this = JInstanceMethod(className, methodName);
        }

        decltype(auto) Invoke(JNIEnv *env, Self self,
                              Args... args) {
            EZ_CORE_ASSERT(id != nullptr, "Constructor not found");

            std::array<jvalue, sizeof...(Args)> jniArgs{
                JTypes::CastToJvalue(args.ToJava())...
            };

            return (env->*Ret::CallInstanceMethodA)(
                self.ToJava(),
                id,
                jniArgs.data());
        }

        consteval static auto GetSignature() {
            return ScriptingEngine::MethodResolver::ResolveSigExact<Ret(Args...)>();
        }

    private:
        jmethodID id = nullptr;
    };

    export template<typename>
    class JStaticMethod {
        static_assert(false, "JConstructor is not specialized");
    };

    export template<typename Ret, typename... Args>
    class JStaticMethod<Ret(Args...)> {
    public:
        JStaticMethod() = default;

        JStaticMethod(jmethodID id) : id(id) {}

        JStaticMethod(const char *className, const char *methodName) {
            auto clazz = Lib::GetClass(className);
            id = ScriptingEngine::GetEnv()->GetStaticMethodID(
                static_cast<jclass>(static_cast<jobject>(clazz)),
                methodName, GetSignature().Data
            );
            EZ_CORE_ASSERT(id != nullptr, "Method not found");
        }

        JStaticMethod(jclass clazz, const char *methodName) {
            id = ScriptingEngine::GetEnv()->GetStaticMethodID(
                clazz,
                methodName, GetSignature().Data
            );

            std::cout << "Signature is: " << GetSignature().Data << std::endl;

            EZ_CORE_ASSERT(id != nullptr, "Method not found");
        }

        void Init(jclass clazz, const char *methodName) {
            *this = JStaticMethod(clazz, methodName);
        }

        void Init(const char *className, const char *methodName) {
            *this = JStaticMethod(className, methodName);
        }

        decltype(auto) Invoke(JNIEnv *env, jclass cls,
                              Args... args) {
            EZ_CORE_ASSERT(id != nullptr, "Constructor not found");

            std::array<jvalue, sizeof...(Args)> jniArgs{
                JTypes::CastToJvalue(args.ToJava())...
            };

            return (env->*Ret::CallStaticMethodA)(
                cls,
                id,
                jniArgs.data());
        }

        consteval static auto GetSignature() {
            return ScriptingEngine::MethodResolver::ResolveSigExact<Ret(Args...)>();
        }

    private:
        jmethodID id = nullptr;
    };
}
