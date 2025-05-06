module;

#include "ComponentsInternal.h"
#include "ScriptableEntityInternal.h"

namespace Easy {
    template<typename T>
    void NativeScriptComponent::Bind() {
        InstantiateScript = []() { return static_cast<ScriptableEntity *>(new T()); };
        DestroyScript = [](NativeScriptComponent *nsc) {
            delete nsc->Instance;
            nsc->Instance = nullptr;
        };
    }
}

export module Easy.Scene.Components;

namespace Easy {
    export using Easy::IDComponent;
    export using Easy::TagComponent;
    export using Easy::TransformComponent;
    export using Easy::SpriteRendererComponent;
    export using Easy::CircleRendererComponent;
    export using Easy::CameraComponent;
    export using Easy::ScriptComponent;
    export using Easy::NativeScriptComponent;
    export using Easy::Rigidbody2DComponent;
    export using Easy::BoxCollider2DComponent;
    export using Easy::CircleCollider2DComponent;
    export using Easy::TextComponent;
    export using Easy::ComponentGroup;
    export using Easy::AllComponents;
}
