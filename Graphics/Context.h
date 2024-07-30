// Author: Jake Rieger
// Created: 7/28/2024.
//

#pragma once

#include "Shared/Platform.h"
#include "Core/Color.h"
#include "Shared/Types.h"

namespace Atom {
    namespace GraphicsContext {
        bool Initialize(HWND hwnd);
        void RequestNewFrame();
        void BeginFrame();
        void EndFrame();
        void Shutdown();
        void Resize(u32 width, u32 height);

        // void DrawRectangle(Vector2 size, Vector2 position, const Color& fillColor);
        // void DrawEllipse(float centerX,
        //                  float centerY,
        //                  float radiusX,
        //                  float radiusY,
        //                  int segments,
        //                  const Color& fillColor);
        // inline void DrawPath() {}
        // inline void DrawBitmap() {}
    };  // namespace GraphicsContext
}  // namespace Atom
