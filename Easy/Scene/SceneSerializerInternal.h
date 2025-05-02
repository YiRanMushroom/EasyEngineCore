#pragma once

#include "SceneInternal.h"

import Easy.Core.Basic;

namespace Easy {

    class SceneSerializer
    {
    public:
        SceneSerializer(const Arc<Scene>& scene);

        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);
    private:
        Arc<Scene> m_Scene;
    };

}