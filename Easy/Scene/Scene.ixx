module;

#include "Core/MacroUtils.hpp"
#include "box2d/b2_world.h"

export module Easy.Scene.Scene;

import Easy.Core.Basic;
import Easy.Core.UUID;
import easy.vendor.entt;
import Easy.Renderer.EditorCamera;
import Easy.Core.Basic;
import Easy.Core.UUID;
import easy.vendor.entt;

import easy.vendor.glm;

import Easy.Core.Basic;
import Easy.Core.UUID;
import Easy.Renderer.Texture;
import Easy.Scene.SceneCamera;
import Easy.Renderer.Font;

namespace Easy {
    export class ScriptableEntity;
    export struct TagComponent;
    export struct IDComponent;
    export class Entity;
    export class Scene;

    class Scene {
    public:
        Scene();

        ~Scene();

        static Arc<Scene> Copy(Arc<Scene> other);

        Entity CreateEntity(const std::string &name = std::string());

        Entity CreateEntityWithUUID(UUID uuid, const std::string &name = std::string());

        void DestroyEntity(Entity entity);

        void OnRuntimeStart();

        void OnRuntimeStop();

        void OnSimulationStart();

        void OnSimulationStop();

        void OnUpdateRuntime(float ts);

        void OnUpdateSimulation(float ts, EditorCamera &camera);

        void OnUpdateEditor(float ts, EditorCamera &camera);

        void OnViewportResize(uint32_t width, uint32_t height);

        Entity DuplicateEntity(Entity entity);

        Entity FindEntityByName(std::string_view name);

        Entity GetEntityByUUID(UUID uuid);

        Entity GetPrimaryCameraEntity();

        bool IsRunning() const { return m_IsRunning; }
        bool IsPaused() const { return m_IsPaused; }

        void SetPaused(bool paused) { m_IsPaused = paused; }

        void Step(int frames = 1);

        template<typename... Components>
        auto GetAllEntitiesWith() {
            return m_Registry.view<Components...>();
        }

    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T &component);

        void OnPhysics2DStart();

        void OnPhysics2DStop();

        void RenderScene(EditorCamera &camera);

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        bool m_IsRunning = false;
        bool m_IsPaused = false;
        int m_StepFrames = 0;

        b2World *m_PhysicsWorld = nullptr;

        std::unordered_map<UUID, entt::entity> m_EntityMap;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

    struct IDComponent {
        UUID ID;

        IDComponent() = default;

        IDComponent(const IDComponent &) = default;
    };

    struct TagComponent {
        std::string Tag;

        TagComponent() = default;

        TagComponent(const TagComponent &) = default;

        TagComponent(const std::string &tag)
            : Tag(tag) {}
    };

    export struct TransformComponent {
        glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

        TransformComponent() = default;

        TransformComponent(const TransformComponent &) = default;

        TransformComponent(const glm::vec3 &translation)
            : Translation(translation) {}

        glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Translation)
                   * rotation
                   * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    export struct SpriteRendererComponent {
        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
        Arc<Texture2D> Texture;
        float TilingFactor = 1.0f;

        SpriteRendererComponent() = default;

        SpriteRendererComponent(const SpriteRendererComponent &) = default;

        SpriteRendererComponent(const glm::vec4 &color)
            : Color(color) {}
    };

    export struct CircleRendererComponent {
        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
        float Thickness = 1.0f;
        float Fade = 0.005f;

        CircleRendererComponent() = default;

        CircleRendererComponent(const CircleRendererComponent &) = default;
    };

    export struct CameraComponent {
        SceneCamera Camera;
        bool Primary = true; // TODO: think about moving to Scene
        bool FixedAspectRatio = false;

        CameraComponent() = default;

        CameraComponent(const CameraComponent &) = default;
    };

    export struct ScriptComponent {
        std::string ClassName;

        ScriptComponent() = default;

        ScriptComponent(const ScriptComponent &) = default;
    };


    export struct NativeScriptComponent {
        ScriptableEntity *Instance = nullptr;

        ScriptableEntity *(*InstantiateScript)();

        void (*DestroyScript)(NativeScriptComponent *);

        template<typename T>
        void Bind();
    };

    // Physics

    struct Rigidbody2DComponent {
        enum class BodyType { Static = 0, Dynamic, Kinematic };

        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        // Storage for runtime
        void *RuntimeBody = nullptr;

        Rigidbody2DComponent() = default;

        Rigidbody2DComponent(const Rigidbody2DComponent &) = default;
    };

    struct BoxCollider2DComponent {
        glm::vec2 Offset = {0.0f, 0.0f};
        glm::vec2 Size = {0.5f, 0.5f};

        // TODO(Yan): move into physics material in the future maybe
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        // Storage for runtime
        void *RuntimeFixture = nullptr;

        BoxCollider2DComponent() = default;

        BoxCollider2DComponent(const BoxCollider2DComponent &) = default;
    };

    struct CircleCollider2DComponent {
        glm::vec2 Offset = {0.0f, 0.0f};
        float Radius = 0.5f;

        // TODO(Yan): move into physics material in the future maybe
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        // Storage for runtime
        void *RuntimeFixture = nullptr;

        CircleCollider2DComponent() = default;

        CircleCollider2DComponent(const CircleCollider2DComponent &) = default;
    };

    export struct TextComponent {
        std::string TextString;
        Arc<Font> FontAsset = Font::GetDefault();
        glm::vec4 Color{1.0f};
        float Kerning = 0.0f;
        float LineSpacing = 0.0f;
    };

    template<typename... Component>
    struct ComponentGroup {};

    using AllComponents =
    ComponentGroup<TransformComponent, SpriteRendererComponent,
        CircleRendererComponent, CameraComponent, ScriptComponent,
        NativeScriptComponent, Rigidbody2DComponent, BoxCollider2DComponent,
        CircleCollider2DComponent, TextComponent>;

    class Entity {
    public:
        Entity() = default;

        Entity(entt::entity handle, Scene *scene);

        Entity(const Entity &other) = default;

        template<typename T, typename... Args>
        T &AddComponent(Args &&... args) {
            HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            T &component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T, typename... Args>
        T &AddOrReplaceComponent(Args &&... args) {
            T &component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T &GetComponent() {
            EZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent() {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent() {
            HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t) m_EntityHandle; }

        UUID GetUUID() { return GetComponent<IDComponent>().ID; }
        const std::string &GetName() { return GetComponent<TagComponent>().Tag; }

        bool operator==(const Entity &other) const {
            return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
        }

        bool operator!=(const Entity &other) const {
            return !(*this == other);
        }

    private:
        entt::entity m_EntityHandle{entt::null};
        Scene *m_Scene = nullptr;
    };

    class ScriptableEntity {
    public:
        virtual ~ScriptableEntity() {}

        template<typename T>
        T &GetComponent() {
            return m_Entity.GetComponent<T>();
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(float ts) {}

    private:
        Entity m_Entity;
        friend class Scene;
    };

    template<typename T>
    void NativeScriptComponent::Bind() {
        InstantiateScript = []() { return static_cast<ScriptableEntity *>(new T()); };
        DestroyScript = [](NativeScriptComponent *nsc) {
            delete nsc->Instance;
            nsc->Instance = nullptr;
        };
    }
}
