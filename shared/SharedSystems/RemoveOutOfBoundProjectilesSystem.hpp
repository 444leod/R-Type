/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RemoveOutOfBoundProjectilesSystem
*/

#ifndef REMOVE_OUT_OF_BOUND_PROJECTILES_SYSTEM_HPP_
#define REMOVE_OUT_OF_BOUND_PROJECTILES_SYSTEM_HPP_

#include <Engine/Systems/AUpdateSystem.hpp>

#include "PremadeComponents/Projectile.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "Config.hpp"

#include <cmath>

class RemoveOutOfBoundProjectilesSystem final : public engine::AUpdateSystem
{
  public:
    explicit RemoveOutOfBoundProjectilesSystem() : AUpdateSystem("RemoveOutOfBoundProjectilesSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Projectile, Velocity, Transform>().each([&](const ecs::Entity& entity, Projectile& projectile, const Velocity& velocity, const Transform& transform) {
        double distanceTraveled = std::sqrt(std::pow(velocity.x * SCALE * deltaTime, 2) + std::pow(velocity.y * SCALE * deltaTime, 2));
        projectile.range -= distanceTraveled;
        if (projectile.range < 0)
            _registry.remove(entity);
        });
    }
};

#endif /* !REMOVE_OUT_OF_BOUND_PROJECTILES_SYSTEM_HPP_ */
