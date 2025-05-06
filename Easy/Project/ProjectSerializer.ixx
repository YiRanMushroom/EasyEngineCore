export module Easy.Project.ProjectSerializer;

import Easy.Core.Basic;
import Easy.Project.Project;

namespace Easy {
    export class ProjectSerializer {
    public:
        ProjectSerializer(Arc<Project> project);

        bool Serialize(const std::filesystem::path &filepath);

        bool Deserialize(const std::filesystem::path &filepath);

    private:
        Arc<Project> m_Project;
    };
}
