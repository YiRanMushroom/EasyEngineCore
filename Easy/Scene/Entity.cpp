import "EntityInternal.h";

namespace Easy {
    Entity::Entity(entt::entity handle, Scene *scene)
        : m_EntityHandle(handle), m_Scene(scene) {}
}
