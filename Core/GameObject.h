// Author: Jake Rieger
// Created: 7/28/2024.
//

#pragma once

#include "Types.h"

namespace Atom {
    class IGameObject {
    public:
        virtual ~IGameObject() = default;
        virtual void Draw()    = 0;

        virtual void Start() {};
        virtual void Update(f64 dT) {};
        virtual void FixedUpdate() {};
        virtual void LateUpdate() {};
        virtual void Destroyed() {};

    protected:
        Vector2 m_Position = {};
        Vector2 m_Size     = {};
        Vector2 m_Rotation = {};
        Vector2 m_Velocity = {};
        bool m_Enabled     = true;
    };
}  // namespace Atom
