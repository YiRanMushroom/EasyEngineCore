export module Easy.Renderer.Shader;

import Easy.Core.Basic;
import easy.vendor.glm;

namespace Easy {
    export class Shader {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void SetInt(const std::string &name, int value) = 0;

        virtual void SetIntArray(const std::string &name, int *values, uint32_t count) = 0;

        virtual void SetFloat(const std::string &name, float value) = 0;

        virtual void SetFloat2(const std::string &name, const glm::vec2 &value) = 0;

        virtual void SetFloat3(const std::string &name, const glm::vec3 &value) = 0;

        virtual void SetFloat4(const std::string &name, const glm::vec4 &value) = 0;

        virtual void SetMat4(const std::string &name, const glm::mat4 &value) = 0;

        [[nodiscard]] virtual const std::string &GetName() const = 0;

        static Arc<Shader> Create(const std::string& filepath);
        static Arc<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    };

    export class ShaderLibrary {
    public:
        void Add(const std::string &name, const Arc<Shader> &shader);

        void Add(const Arc<Shader> &shader);

        Arc<Shader> Load(const std::string &filepath);

        Arc<Shader> Load(const std::string &name, const std::string &filepath);

        Arc<Shader> Get(const std::string &name);

        bool Exists(const std::string &name) const;

    private:
        std::unordered_map<std::string, Arc<Shader>> m_Shaders;
    };
}
