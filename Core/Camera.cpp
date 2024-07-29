// Author: Jake Rieger
// Created: 7/29/2024.
//

#include "Camera.h"

namespace Atom {
    glm::mat4 Camera::GetProjectionMatrix(const Vector2& screenSize) {
        return glm::ortho(0.f, screenSize.X, screenSize.Y, 0.f, -1.0f, 1.0f);
    }

    glm::vec4 Camera::GetScreenCoords(const glm::vec4 ndc, const glm::mat4& proj) {
        return proj * ndc;
    }
}  // namespace Atom