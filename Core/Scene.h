// Author: Jake Rieger
// Created: 7/27/2024.
//

#pragma once
#include "../Shared/Types.h"

namespace Atom {

    class Scene {
    public:
        Scene() = default;

        void Start() {}
        void Update(f64 dT) {}
        void FixedUpdate() {}
        void LateUpdate() {}
        void Destroyed() {}
    };

}  // namespace Atom
