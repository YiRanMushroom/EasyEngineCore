#pragma once

import Easy.Core.Basic;
import Easy.Core.UUID;
import Easy.Renderer.EditorCamera;

import Easy.Vendor.entt;

class b2World;

namespace Easy {

    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        static Arc<Scene> Copy(Arc<Scene> other);

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnRuntimeStart();
        void OnRuntimeStop();

        void OnSimulationStart();
        void OnSimulationStop();

        void OnUpdateRuntime(float ts);
        void OnUpdateSimulation(float ts, EditorCamera& camera);
        void OnUpdateEditor(float ts, EditorCamera& camera);
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
        auto GetAllEntitiesWith()
        {
            return m_Registry.view<Components...>();
        }
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);

        void OnPhysics2DStart();
        void OnPhysics2DStop();

        void RenderScene(EditorCamera& camera);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        bool m_IsRunning = false;
        bool m_IsPaused = false;
        int m_StepFrames = 0;

        b2World* m_PhysicsWorld = nullptr;

        std::unordered_map<UUID, entt::entity> m_EntityMap;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

}
