// Author: Jake Rieger
// Created: 7/28/2024.
//

#pragma once

#include "Platform.h"
#include "Core/Color.h"
#include "Core/Types.h"

namespace Atom {
    namespace GraphicsContext {
        bool Initialize(HWND hwnd);
        void BeginFrame();
        void EndFrame(HWND hwnd);
        void Resize(u32 width, u32 height);

        u32 CreateVAO(const std::vector<f32>& vertices, const std::vector<u32>& indices);

        void DrawRectangle(Vector2 size, Vector2 position, Color color, u32 shader);
        inline void DrawEllipse() {}
        inline void DrawPath() {}
        inline void DrawString() {}
        inline void DrawBitmap() {}
    };  // namespace GraphicsContext
}  // namespace Atom
