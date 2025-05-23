module;

#include <Core/MacroUtils.hpp>

export module Easy.Renderer.Buffer;

import Easy.Core.Basic;

namespace Easy {
    export enum class ShaderDataType : uint8_t {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float: return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3: return 4 * 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4 * 4;
            case ShaderDataType::Int: return 4;
            case ShaderDataType::Int2: return 4 * 2;
            case ShaderDataType::Int3: return 4 * 3;
            case ShaderDataType::Int4: return 4 * 4;
            case ShaderDataType::Bool: return 1;
            default: EZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
        }
        return 0;
    }

    export struct BufferElement {
        std::optional<std::string> Name;
        ShaderDataType Type;
        uint32_t Size;
        size_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::optional<std::string> &name = "<anonymous>",
                      bool normalized = false)
            : Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

        [[nodiscard]] uint32_t GetComponentCount() const {
            switch (Type) {
                case ShaderDataType::Float: return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3: return 3; // 3* float3
                case ShaderDataType::Mat4: return 4; // 4* float4
                case ShaderDataType::Int: return 1;
                case ShaderDataType::Int2: return 2;
                case ShaderDataType::Int3: return 3;
                case ShaderDataType::Int4: return 4;
                case ShaderDataType::Bool: return 1;
                default: EZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
            return 0;
        }
    };

    export class BufferLayout {
    public:
        BufferLayout() = default;

        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_Elements(std::move(elements)) {
            CalculateOffsetsAndStride();
        }

        [[nodiscard]] uint32_t GetStride() const { return m_Stride; }
        [[nodiscard]] const std::vector<BufferElement> &GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        [[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride() {
            size_t offset = 0;
            m_Stride = 0;
            for (auto &element: m_Elements) {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    export class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void SetData(const void *data, uint32_t size) = 0;

        [[nodiscard]] virtual const BufferLayout &GetLayout() const = 0;

        virtual void SetLayout(const BufferLayout &layout) = 0;

        static Arc<VertexBuffer> Create(uint32_t size);
        static Arc<VertexBuffer> Create(const float *vertices, uint32_t size);
    };

    export class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        [[nodiscard]] virtual uint32_t GetCount() const = 0;

        static Arc<IndexBuffer> Create(const uint32_t *indices, uint32_t size);
    };
}
