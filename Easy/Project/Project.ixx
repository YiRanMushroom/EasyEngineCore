module;

export module Easy.Project.Project;

import <Core/MacroUtils.hpp>;
import Easy.Core.Basic;

namespace Easy {

    export struct ProjectConfig
    {
        std::string Name = "Untitled";

        std::filesystem::path StartScene;

        std::filesystem::path AssetDirectory;
        std::filesystem::path ScriptModulePath;
    };

    export class Project
    {
    public:
        static const std::filesystem::path& GetProjectDirectory()
        {
            EZ_CORE_ASSERT(s_ActiveProject);
            return s_ActiveProject->m_ProjectDirectory;
        }

        static std::filesystem::path GetAssetDirectory()
        {
            EZ_CORE_ASSERT(s_ActiveProject);
            return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
        }

        // TODO(Yan): move to asset manager when we have one
        static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
        {
            EZ_CORE_ASSERT(s_ActiveProject);
            return GetAssetDirectory() / path;
        }

        ProjectConfig& GetConfig() { return m_Config; }

        static Arc<Project> GetActive() { return s_ActiveProject; }

        static Arc<Project> New();
        static Arc<Project> Load(const std::filesystem::path& path);
        static bool SaveActive(const std::filesystem::path& path);
    private:
        ProjectConfig m_Config;
        std::filesystem::path m_ProjectDirectory;

        inline static Arc<Project> s_ActiveProject;
    };

}