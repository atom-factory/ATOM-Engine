// Author: Jake Rieger
// Created: 7/29/2024.
//

#pragma once

#include "Types.h"

#include <freetype-gl/freetype-gl.h>

namespace Atom {
    class Font {
    public:
        Font(const char* ttfFile, f32 size);
        ~Font();
        void Resize(f32 size);

        [[nodiscard]] texture_font_t* GetTextureFont() const {
            return m_Font;
        }

        [[nodiscard]] texture_atlas_t* GetAtlas() const {
            return m_Atlas;
        }

    private:
        const char* m_TTFFile;
        f32 m_Size;
        texture_atlas_t* m_Atlas = nullptr;
        texture_font_t* m_Font   = nullptr;
    };
}  // namespace Atom
