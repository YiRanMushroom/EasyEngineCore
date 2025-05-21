module;

#undef INFINITE
import "msdf-atlas-gen.h";

export module Easy.Renderer.MSDFData;

import Easy.Core.Basic;

namespace Easy {
    export struct MSDFData {
        std::vector<msdf_atlas::GlyphGeometry> Glyphs;
        msdf_atlas::FontGeometry FontGeometry;
    };
}
