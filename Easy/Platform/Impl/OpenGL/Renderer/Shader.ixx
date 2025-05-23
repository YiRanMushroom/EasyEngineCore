

export module Easy.Platform.Impl.OpenGL.Renderer.Shader;

import Easy.Renderer.Shader;
import Easy.Renderer.VertexArray;
import Easy.Core.Basic;
import Easy.Vendor.glm;
import Easy.Core.Util;

namespace Easy {
    export class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string &filepath);

        OpenGLShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);

        virtual ~OpenGLShader() override = default;

        virtual void Bind() const override;

        virtual void Unbind() const override;

        virtual void SetInt(const std::string &name, int value) override;

        virtual void SetIntArray(const std::string &name, int *values, uint32_t count) override;

        virtual void SetFloat(const std::string &name, float value) override;

        virtual void SetFloat2(const std::string &name, const glm::vec2 &value) override;

        virtual void SetFloat3(const std::string &name, const glm::vec3 &value) override;

        virtual void SetFloat4(const std::string &name, const glm::vec4 &value) override;

        virtual void SetMat4(const std::string &name, const glm::mat4 &value) override;

        virtual const std::string &GetName() const override { return m_Name; }

        void UploadUniformInt(const std::string &name, int value);

        void UploadUniformIntArray(const std::string &name, int *values, uint32_t count);

        void UploadUniformFloat(const std::string &name, float value);

        void UploadUniformFloat2(const std::string &name, const glm::vec2 &value);

        void UploadUniformFloat3(const std::string &name, const glm::vec3 &value);

        void UploadUniformFloat4(const std::string &name, const glm::vec4 &value);

        void UploadUniformMat3(const std::string &name, const glm::mat3 &matrix);

        void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

    private:
        std::string ReadFile(const std::string &filepath);

        std::unordered_map<uint32_t, std::string> PreProcess(const std::string &source);

        void CompileOrGetVulkanBinaries(const std::unordered_map<uint32_t, std::string> &shaderSources);

        void CompileOrGetOpenGLBinaries();

        void CreateProgram();

        void Reflect(uint32_t stage, const std::vector<uint32_t> &shaderData);

    private:
        struct ShaderDeleter {
            void operator()(uint32_t shader) const;
        };

        unique_owner_default<uint32_t, ShaderDeleter, 0> m_RendererID;
        std::string m_FilePath;
        std::string m_Name;

        std::unordered_map<uint32_t, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSPIRV;

        std::unordered_map<uint32_t, std::string> m_OpenGLSourceCode;
    };
}
