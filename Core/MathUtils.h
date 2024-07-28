// Author: Jake Rieger
// Created: 7/28/2024.
//

#pragma once

#include "Types.h"

namespace Atom::Math {
    template<typename T>
    T Lerp(T a, T b, f64 t) {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");

        if (a == b) {
            return a;
        }

        return static_cast<T>(a * (1.0 - t) + b * t);
    }
}  // namespace Atom::Math