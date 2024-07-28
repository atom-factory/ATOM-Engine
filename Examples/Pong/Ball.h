// Author: Jake Rieger
// Created: 7/28/2024.
//

#pragma once

#include "GameObject.h"

class Ball final : public Atom::IGameObject {
public:
    Ball() = default;
    void Draw() override;
};
