// Author: Jake Rieger
// Created: 7/29/2024.
//

#include "Font.h"
#include "IO.h"

namespace Atom {
    Font::Font(const char* ttfFile, const f32 size) : m_TTFFile(ttfFile), m_Size(size) {
        m_Atlas = texture_atlas_new(512, 512, 1);
        m_Font  = texture_font_new_from_file(m_Atlas, size, ttfFile);
    }

    Font::~Font() {
        texture_font_delete(m_Font);
        texture_atlas_delete(m_Atlas);
    }

    void Font::Resize(const f32 size) {
        m_Size = size;
        texture_font_delete(m_Font);
        m_Font = texture_font_new_from_file(m_Atlas, size, m_TTFFile);
    }
}  // namespace Atom