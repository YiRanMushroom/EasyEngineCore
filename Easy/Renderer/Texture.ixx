module;

export module Easy.Renderer.Texture;

import Easy.Core.Basic;
import Easy.Renderer.RendererAPI;

namespace Easy {
    export enum class ImageFormat {
        None = 0,
        R8,
        RGB8,
        RGBA8,
        RGBA32F
    };

    export struct TextureSpecification {
        uint32_t Width = 1;
        uint32_t Height = 1;
        ImageFormat Format = ImageFormat::RGBA8;
        bool GenerateMips = true;
    };

    export class Texture {
    public:
        virtual ~Texture() = default;

        virtual const TextureSpecification &GetSpecification() const = 0;

        virtual uint32_t GetWidth() const = 0;

        virtual uint32_t GetHeight() const = 0;

        virtual const std::string &GetPath() const = 0;

        virtual void SetData(void *data, uint32_t size) = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;

        virtual bool IsLoaded() const = 0;

        virtual bool operator==(const Texture &other) const = 0;

        virtual Easy::RendererAPI::API GetAPI() const = 0;
    };

    export class Texture2D : public Texture {
    public:
        static Arc<Texture2D> Create(const TextureSpecification &specification);

        static Arc<Texture2D> Create(const std::string &path);
    };
}
