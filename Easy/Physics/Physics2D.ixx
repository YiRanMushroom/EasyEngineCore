module;

#include "Scene/ComponentsInternal.h"
#include "box2d/b2_body.h"
#include "Core/MacroUtils.hpp"

export module Easy.Physics.Physics2D;


namespace Easy::Utils {
    export b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType) {
        switch (bodyType) {
            case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
            case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
            case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }

        EZ_CORE_ASSERT(false, "Unknown body type");
        return b2_staticBody;
    }

    export Rigidbody2DComponent::BodyType Rigidbody2DTypeFromBox2DBody(b2BodyType bodyType) {
        switch (bodyType) {
            case b2_staticBody: return Rigidbody2DComponent::BodyType::Static;
            case b2_dynamicBody: return Rigidbody2DComponent::BodyType::Dynamic;
            case b2_kinematicBody: return Rigidbody2DComponent::BodyType::Kinematic;
        }

        EZ_CORE_ASSERT(false, "Unknown body type");
        return Rigidbody2DComponent::BodyType::Static;
    }
}
