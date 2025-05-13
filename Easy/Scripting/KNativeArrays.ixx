module;
#include "Core/MacroUtils.hpp"
export module Easy.Scripting.KNativeArrays;

import Easy.Core.Basic;
import Easy.Scripting.JniBind;
import Easy.Scripting.JTypes;
import Easy.Scripting.NativeBuffer;
import Easy.Core.Util;

using namespace Easy::ScriptingEngine::JTypes;
using namespace jni;

namespace Easy::ScriptingEngine {
    class KNativeWStringImpl : public AutoManagedBufferBase {
    public:
        std::wstring& GetNativeString() {
            return m_NativeString;
        }

        explicit KNativeWStringImpl(std::wstring_view str) : m_NativeString(str) {}

        explicit KNativeWStringImpl(const wchar_t* str) : m_NativeString(str) {}

        explicit KNativeWStringImpl(std::wstring&& str) : m_NativeString(std::move(str)) {}

        KNativeWStringImpl() = default;

        virtual ~KNativeWStringImpl() override = default;

    private:
        std::wstring m_NativeString{};
    };

    export class KNativeWString : public InjectJObject {
    public:
        constexpr static StringLiteral SimpleName = "com/easy/NativeWString";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        constexpr static Class Definition {
            SimpleName.Data,
            Constructor{jlong{}},
            Method{"GetNativeAddress", Return{jlong{}}, Params{}},
        };

    public:
        [[nodiscard]] KNativeWStringImpl* GetAddress() const {
            return
            static_cast<KNativeWStringImpl *>(reinterpret_cast<AutoManagedBufferBase*> // NOLINT
                (m_ObjectProvider.GetObject().Call<"GetNativeAddress">()));
        }

        KNativeWString() = default;

        template<typename T>
        KNativeWString(T, jobject obj) : m_ObjectProvider(T{}, obj) {}

        KNativeWString(std::wstring_view str) {
            std::unique_ptr<KNativeWStringImpl> nativeString = std::make_unique<KNativeWStringImpl>(str);
            nativeString->GetNativeString() = str;
            LocalObject<Definition> obj = LocalObject<Definition>{
                reinterpret_cast<jlong>(static_cast<AutoManagedBufferBase*>(nativeString.release()))};

            m_ObjectProvider = LocalObjectProvider<Definition>{PromoteToGlobal{}, static_cast<jobject>(obj)};
        }

        KNativeWString(const wchar_t* str) : KNativeWString(std::wstring_view(str)) {}

        KNativeWString(std::wstring&& str) {
            std::unique_ptr<KNativeWStringImpl> nativeString = std::make_unique<KNativeWStringImpl>();
            nativeString->GetNativeString() = std::move(str);
            LocalObject<Definition> obj = LocalObject<Definition>{
                reinterpret_cast<jlong>(static_cast<AutoManagedBufferBase*>(nativeString.release()))};

            m_ObjectProvider = LocalObjectProvider<Definition>{PromoteToGlobal{}, static_cast<jobject>(obj)};
        }

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

    public:
        size_t Length() const {
            return GetAddress()->GetNativeString().length();
        }

        wchar_t& operator[](size_t index) const {
            return GetAddress()->GetNativeString()[index];
        }

        std::wstring& Get() const {
            return GetAddress()->GetNativeString();
        }

    private:
        static Jint LengthImpl(KNativeWString obj) {
            return static_cast<jint>(obj.Length());
        }

        static Jchar GetIndexImpl(KNativeWString obj, Jint index) {
            return obj[index.Get()];
        }

        static void SetIndexImpl(KNativeWString obj, Jint index, Jchar value) {
            obj.GetAddress()->GetNativeString()[index.Get()] = value.Get();
        }

        static Jlong CreateNativeFromStringImpl(JString str) {
            const std::string &string = str.Get();
            std::wstring wstr(string.begin(), string.end());
            return reinterpret_cast<jlong>(
                static_cast<AutoManagedBufferBase*>(new KNativeWStringImpl(std::move(wstr)))
            );
        }

    public:
        void static Init() {
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeInstanceMethodStatic<&LengthImpl>(
                SimpleName.Data, "Length"));
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeInstanceMethodStatic<&GetIndexImpl>(
                SimpleName.Data, "GetIndex"));
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeInstanceMethodStatic<&SetIndexImpl>(
                SimpleName.Data, "SetIndex"));
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<&CreateNativeFromStringImpl>(
                SimpleName.Data, "CreateNativeFromString"));
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
    };

    class KNativeStringImpl : public AutoManagedBufferBase {
    public:
        std::string& GetNativeString() {
            return m_NativeString;
        }

        explicit KNativeStringImpl(std::string_view str) : m_NativeString(str) {}

        explicit KNativeStringImpl(const char* str) : m_NativeString(str) {}

        explicit KNativeStringImpl(std::string&& str) : m_NativeString(std::move(str)) {}

        KNativeStringImpl() = default;

        virtual ~KNativeStringImpl() override = default;

    private:
        std::string m_NativeString{};
    };

    export class KNativeString : public InjectJObject {
    public:
        constexpr static StringLiteral SimpleName = "com/easy/NativeString";
        constexpr static StringLiteral FullName = MakeFullName(SimpleName);
        constexpr static Class Definition {
            SimpleName.Data,
            Constructor{jlong{}},
            Method{"GetNativeAddress", Return{jlong{}}, Params{}},
        };
    public:
        [[nodiscard]] KNativeStringImpl* GetAddress() const {
            return
            static_cast<KNativeStringImpl *>(reinterpret_cast<AutoManagedBufferBase*> // NOLINT
                (m_ObjectProvider.GetObject().Call<"GetNativeAddress">()));
        }

        KNativeString() = default;

        template<typename T>
        KNativeString(T, jobject obj) : m_ObjectProvider(T{}, obj) {}

        KNativeString(std::string_view str) {
            std::unique_ptr<KNativeStringImpl> nativeString = std::make_unique<KNativeStringImpl>(str);
            nativeString->GetNativeString() = str;
            LocalObject<Definition> obj = LocalObject<Definition>{
                reinterpret_cast<jlong>(static_cast<AutoManagedBufferBase*>(nativeString.release()))};

            m_ObjectProvider = LocalObjectProvider<Definition>{PromoteToGlobal{}, static_cast<jobject>(obj)};
        }

        KNativeString(const char* str) : KNativeString(std::string_view(str)) {}

        KNativeString(std::string&& str) {
            std::unique_ptr<KNativeStringImpl> nativeString = std::make_unique<KNativeStringImpl>();
            nativeString->GetNativeString() = std::move(str);
            LocalObject<Definition> obj = LocalObject<Definition>{
                reinterpret_cast<jlong>(static_cast<AutoManagedBufferBase*>(nativeString.release()))};

            m_ObjectProvider = LocalObjectProvider<Definition>{PromoteToGlobal{}, static_cast<jobject>(obj)};
        }

        [[nodiscard]] jobject ToJava() const {
            return m_ObjectProvider.GetRawObject();
        }

    public:
        size_t Length() const {
            return GetAddress()->GetNativeString().length();
        }

        char& operator[](size_t index) const {
            return GetAddress()->GetNativeString()[index];
        }

        std::string& Get() const {
            return GetAddress()->GetNativeString();
        }

    private:
        static Jint LengthImpl(KNativeString obj) {
            return static_cast<jint>(obj.Length());
        }

        static Jchar GetIndexImpl(KNativeString obj, Jint index) {
            return static_cast<uint16_t>(obj[index.Get()]);
        }

        static void SetIndexImpl(KNativeString obj, Jint index, Jchar value) {
            obj.GetAddress()->GetNativeString()[index.Get()] = static_cast<char>(value.Get());
        }

        static Jlong CreateNativeFromStringImpl(JString str) {
            const std::string &string = str.Get();
            return reinterpret_cast<jlong>(
                static_cast<AutoManagedBufferBase*>(new KNativeStringImpl(string))
            );
        }

    public:
        static void Init() {
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeInstanceMethodStatic<&LengthImpl>(
                SimpleName.Data, "Length"));
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeInstanceMethodStatic<&GetIndexImpl>(
                SimpleName.Data, "GetIndex"));
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeInstanceMethodStatic<&SetIndexImpl>(
                SimpleName.Data, "SetIndex"));
            EZ_CORE_ASSERT(ScriptingEngine::RegisterNativeStaticMethodStatic<&CreateNativeFromStringImpl>(
                SimpleName.Data, "CreateNativeFromString"));
        }

    private:
        LocalObjectProvider<Definition> m_ObjectProvider{};
    };
}