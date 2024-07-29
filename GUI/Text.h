// Author: Jake Rieger
// Created: 7/29/2024.
//

#pragma once

#include "Font.h"
#include "Types.h"
#include "Color.h"
#include "Graphics/Shader.h"

#include <freetype-gl/vertex-buffer.h>

namespace Atom {
    class Text {
    public:
        Text(const char* ttfFile, f32 size);
        ~Text();
        void Draw(const str& value, const Vector2& position, const Color& color) const;

    private:
        Font* m_Font;
        vertex_buffer_t* m_VertexBuffer = nullptr;
        Shader* m_Shader                = nullptr;
    };
}  // namespace Atom
