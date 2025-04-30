module;

export module Easy.Renderer.Font;

import Easy.Core.Basic;
import Easy.Renderer.Texture;
import Easy.Renderer.MSDFData;

namespace Easy {

    export class Font {
    public:
        Font(const std::filesystem::path &font);

        ~Font();

        const MSDFData *GetMSDFData() const { return m_Data; }
        Arc<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

        static Arc<Font> GetDefault();

    private:
        MSDFData *m_Data;
        Arc<Texture2D> m_AtlasTexture;
    };
}
