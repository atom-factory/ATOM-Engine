// Author: Jake Rieger
// Created: 7/29/2024.
//

#include "Text.h"

#include <GL/glew.h>

namespace Atom {
    auto kVertexShader = R"(
        #version 460 core
        layout(location = 0) in vec3 vertex;
        layout(location = 1) in vec2 tex_coord;
        layout(location = 2) in vec4 color;

        out vec2 TexCoord;
        out vec4 VertexColor;

        void main() {
            gl_Position = vec4(vertex, 1.0);
            TexCoord = tex_coord;
            VertexColor = color;
        }
    )";

    auto kFragmentShader = R"(
        #version 460 core
        in vec2 TexCoord;
        in vec4 VertexColor;

        out vec4 FragColor;

        uniform sampler2D text;

        void main() {
            FragColor = texture(text, TexCoord) * VertexColor;
        }
    )";

    struct vertex_t {
        float x, y, z;  // Vertex position
        float s, t;     // Texture coordinates
        vec4 color;     // Color (vec4 is assumed to be a float[4])
    };

    Text::Text(const char* ttfFile, const f32 size) {
        m_Font         = new Font(ttfFile, size);
        m_Shader       = Shader::Create(kVertexShader, kFragmentShader);
        m_VertexBuffer = vertex_buffer_new("vertex:3f,tex_coord:2f,color:4f");
    }

    Text::~Text() {
        delete m_Font;
        delete m_Shader;
        vertex_buffer_delete(m_VertexBuffer);
    }

    void Text::Draw(const str& value, const Vector2& position, const Color& color) const {
        glUseProgram(m_Shader->GetProgram());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Font->GetAtlas()->id);
        m_Shader->SetInt("text", 0);

        const auto font = m_Font->GetTextureFont();
        vec2 pen        = {position.X, position.Y};
        texture_font_load_glyphs(font, value.c_str());

        for (const char* p = value.c_str(); *p != '\0'; p++) {
            const texture_glyph_t* glyph = texture_font_get_glyph(font, p);
            if (glyph) {
                float x0 = pen.x + glyph->offset_x;
                float y0 = pen.y + glyph->offset_y;
                float x1 = x0 + glyph->width;
                float y1 = y0 - glyph->height;

                float s0 = glyph->s0;
                float t0 = glyph->t0;
                float s1 = glyph->s1;
                float t1 = glyph->t1;

                GLuint indices[]    = {0, 1, 2, 0, 2, 3};
                const vec4 _color   = {1, 1, 1, 1};
                vertex_t vertices[] = {{x0, y0, 0, s0, t0, _color},
                                       {x0, y1, 0, s0, t1, _color},
                                       {x1, y1, 0, s1, t1, _color},
                                       {x1, y0, 0, s1, t0, _color}};

                vertex_buffer_push_back(m_VertexBuffer, vertices, 4, indices, 6);
                pen.x += glyph->advance_x;
            }
        }

        vertex_buffer_render(m_VertexBuffer, GL_TRIANGLES);
    }
}  // namespace Atom