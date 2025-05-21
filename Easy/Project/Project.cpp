module;

import "Core/MacroUtils.hpp";

module Easy.Project.Project;

import Easy.Core.Basic;
import Easy.Project.ProjectSerializer;

namespace Easy {

    Arc<Project> Project::New()
    {
        s_ActiveProject = MakeArc<Project>();
        return s_ActiveProject;
    }

    Arc<Project> Project::Load(const std::filesystem::path& path)
    {
        Arc<Project> project = MakeArc<Project>();

        ProjectSerializer serializer(project);
        if (serializer.Deserialize(path))
        {
            project->m_ProjectDirectory = path.parent_path();
            s_ActiveProject = project;
            return s_ActiveProject;
        }

        return nullptr;
    }

    bool Project::SaveActive(const std::filesystem::path& path)
    {
        ProjectSerializer serializer(s_ActiveProject);
        if (serializer.Serialize(path))
        {
            s_ActiveProject->m_ProjectDirectory = path.parent_path();
            return true;
        }

        return false;
    }

}