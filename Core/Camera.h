// Author: Jake Rieger
// Created: 7/29/2024.
//

#pragma once

#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "../Shared/Types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Atom {
    class Camera {
    public:
        static glm::mat4 GetProjectionMatrix(const Vector2& screenSize);
        static glm::vec4 GetScreenCoords(glm::vec4 ndc, const glm::mat4& proj);
    };
}  // namespace Atom
