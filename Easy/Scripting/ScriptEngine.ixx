module;

// #include "mono/metadata/object.h"
// #include "mono/metadata/reflection.h"

extern "C" {
    struct _MonoClass;
    struct _MonoObject;
    struct _MonoMethod;
    struct _MonoAssembly;
    struct _MonoImage;
    struct _MonoClassField;
    struct _MonoString;
}

#include <cstring>

#include "Scene/SceneInternal.h"
#include "Scene/EntityInternal.h"

export module Easy.Script.ScriptingEngine;

import Easy.Core.Basic;
import Easy.Scripting.ScriptGlue;

export {
    using MonoClass = _MonoClass;
    using MonoObject = _MonoObject;
    using MonoMethod = _MonoMethod;
    using MonoAssembly = _MonoAssembly;
    using MonoImage = _MonoImage;
    using MonoClassField = _MonoClassField;
    using MonoString = _MonoString;
}

namespace Easy {
    export class ScriptEngine;
    export class ScriptInstance;

    export enum class ScriptFieldType {
        None = 0,
        Float, Double,
        Bool, Char, Byte, Short, Int, Long,
        UByte, UShort, UInt, ULong,
        Vector2, Vector3, Vector4,
        Entity
    };

    export struct ScriptField {
        ScriptFieldType Type;
        std::string Name;

        MonoClassField *ClassField;
    };

    // ScriptField + data storage
    export struct ScriptFieldInstance {
        ScriptField Field;

        ScriptFieldInstance() {
            memset(m_Buffer, 0, sizeof(m_Buffer));
        }

        template<typename T>
        T GetValue() {
            static_assert(sizeof(T) <= 16, "Type too large!");
            return *(T *) m_Buffer;
        }

        template<typename T>
        void SetValue(T value) {
            static_assert(sizeof(T) <= 16, "Type too large!");
            memcpy(m_Buffer, &value, sizeof(T));
        }

    private:
        uint8_t m_Buffer[16];

        friend class ScriptEngine;
        friend class ScriptInstance;
    };

    export using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

    export class ScriptClass {
    public:
        ScriptClass() = default;

        ScriptClass(const std::string &classNamespace, const std::string &className, bool isCore = false);

        MonoObject *Instantiate();

        MonoMethod *GetMethod(const std::string &name, int parameterCount);

        MonoObject *InvokeMethod(MonoObject *instance, MonoMethod *method, void **params = nullptr);

        [[nodiscard]] const std::map<std::string, ScriptField> &GetFields() const { return m_Fields; }

    private:
        std::string m_ClassNamespace;
        std::string m_ClassName;

        std::map<std::string, ScriptField> m_Fields;

        MonoClass *m_MonoClass = nullptr;

        friend class ScriptEngine;
    };

    class ScriptInstance {
    public:
        ScriptInstance(Arc<ScriptClass> scriptClass, Entity entity);

        void InvokeOnCreate();

        void InvokeOnUpdate(float ts);

        Arc<ScriptClass> GetScriptClass() { return m_ScriptClass; }

        template<typename T>
        T GetFieldValue(const std::string &name) {
            static_assert(sizeof(T) <= 16, "Type too large!");

            bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
            if (!success)
                return T();

            return *(T *) s_FieldValueBuffer;
        }

        template<typename T>
        void SetFieldValue(const std::string &name, T value) {
            static_assert(sizeof(T) <= 16, "Type too large!");

            SetFieldValueInternal(name, &value);
        }

        MonoObject *GetManagedObject() { return m_Instance; }

    private:
        bool GetFieldValueInternal(const std::string &name, void *buffer);

        bool SetFieldValueInternal(const std::string &name, const void *value);

    private:
        Arc<ScriptClass> m_ScriptClass;

        MonoObject *m_Instance = nullptr;
        MonoMethod *m_Constructor = nullptr;
        MonoMethod *m_OnCreateMethod = nullptr;
        MonoMethod *m_OnUpdateMethod = nullptr;

        inline static char s_FieldValueBuffer[16];

        friend class ScriptEngine;
        friend struct ScriptFieldInstance;
    };

    class ScriptEngine {
    public:
        static void Init();

        static void Shutdown();

        static bool LoadAssembly(const std::filesystem::path &filepath);

        static bool LoadAppAssembly(const std::filesystem::path &filepath);

        static void ReloadAssembly();

        static void OnRuntimeStart(Scene *scene);

        static void OnRuntimeStop();

        static bool EntityClassExists(const std::string &fullClassName);

        static void OnCreateEntity(Entity entity);

        static void OnUpdateEntity(Entity entity, Timestep ts);

        static Scene *GetSceneContext();

        static Arc<ScriptInstance> GetEntityScriptInstance(UUID entityID);

        static Arc<ScriptClass> GetEntityClass(const std::string &name);

        static std::unordered_map<std::string, Arc<ScriptClass>> GetEntityClasses();

        static ScriptFieldMap &GetScriptFieldMap(Entity entity);

        static MonoImage *GetCoreAssemblyImage();

        static MonoObject *GetManagedInstance(UUID uuid);

        static MonoString *CreateString(const char *string);

    private:
        static void InitMono();

        static void ShutdownMono();

        static MonoObject *InstantiateClass(MonoClass *monoClass);

        static void LoadAssemblyClasses();

        friend class ScriptClass;
        friend class ScriptGlue;
    };

    namespace Utils {
        export const char *ScriptFieldTypeToString(ScriptFieldType fieldType) {
            switch (fieldType) {
                case ScriptFieldType::None: return "None";
                case ScriptFieldType::Float: return "Float";
                case ScriptFieldType::Double: return "Double";
                case ScriptFieldType::Bool: return "Bool";
                case ScriptFieldType::Char: return "Char";
                case ScriptFieldType::Byte: return "Byte";
                case ScriptFieldType::Short: return "Short";
                case ScriptFieldType::Int: return "Int";
                case ScriptFieldType::Long: return "Long";
                case ScriptFieldType::UByte: return "UByte";
                case ScriptFieldType::UShort: return "UShort";
                case ScriptFieldType::UInt: return "UInt";
                case ScriptFieldType::ULong: return "ULong";
                case ScriptFieldType::Vector2: return "Vector2";
                case ScriptFieldType::Vector3: return "Vector3";
                case ScriptFieldType::Vector4: return "Vector4";
                case ScriptFieldType::Entity: return "Entity";
            }
            EZ_CORE_ASSERT(false, "Unknown ScriptFieldType");
            return "None";
        }

        export ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType) {
            if (fieldType == "None") return ScriptFieldType::None;
            if (fieldType == "Float") return ScriptFieldType::Float;
            if (fieldType == "Double") return ScriptFieldType::Double;
            if (fieldType == "Bool") return ScriptFieldType::Bool;
            if (fieldType == "Char") return ScriptFieldType::Char;
            if (fieldType == "Byte") return ScriptFieldType::Byte;
            if (fieldType == "Short") return ScriptFieldType::Short;
            if (fieldType == "Int") return ScriptFieldType::Int;
            if (fieldType == "Long") return ScriptFieldType::Long;
            if (fieldType == "UByte") return ScriptFieldType::UByte;
            if (fieldType == "UShort") return ScriptFieldType::UShort;
            if (fieldType == "UInt") return ScriptFieldType::UInt;
            if (fieldType == "ULong") return ScriptFieldType::ULong;
            if (fieldType == "Vector2") return ScriptFieldType::Vector2;
            if (fieldType == "Vector3") return ScriptFieldType::Vector3;
            if (fieldType == "Vector4") return ScriptFieldType::Vector4;
            if (fieldType == "Entity") return ScriptFieldType::Entity;

            EZ_CORE_ASSERT(false, "Unknown ScriptFieldType");
            return ScriptFieldType::None;
        }
    }
}
