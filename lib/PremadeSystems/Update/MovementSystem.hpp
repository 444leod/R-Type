/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MovementSystem
*/

#ifndef MOVEMENT_SYSTEM_HPP_
#define MOVEMENT_SYSTEM_HPP_

#include <Engine/Systems/AUpdateSystem.hpp>

#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "Config.hpp"

class MovementSystem final : public engine::AUpdateSystem
{
  public:
    explicit MovementSystem() : AUpdateSystem("MovementSystem") {}

    void execute(const double& deltaTime) override
    {
        _registry.view<Transform, Velocity>().each(
            [deltaTime](Transform& transform, const Velocity& velocity)
            {
                transform.x += static_cast<float>((velocity.x * SCALE) * deltaTime);
                transform.y += static_cast<float>((velocity.y * SCALE) * deltaTime);
            });
    }
};

#endif /* !MOVEMENT_SYSTEM_HPP_ */
