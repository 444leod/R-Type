/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RemoveOutOfBoundProjectilesSystem
*/

#ifndef REMOVEOUTOFBOUNDPROJECTILESSYSTEM_HPP_
#define REMOVEOUTOFBOUNDPROJECTILESSYSTEM_HPP_

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"
#include "Config.hpp"

class RemoveOutOfBoundProjectilesSystem final : public AUpdateSystem
{
public:
    explicit RemoveOutOfBoundProjectilesSystem(ecs::Registry &registry) : AUpdateSystem(registry, "RemoveOutOfBoundProjectilesSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Projectile, Velocity, Transform>().each([&](const Entity& entity, Projectile& projectile, const Velocity& velocity, const Transform& transform) {
        double distanceTraveled = std::sqrt(std::pow(velocity.x * SCALE * deltaTime, 2) + std::pow(velocity.y * SCALE * deltaTime, 2));
        projectile.range -= distanceTraveled;
        if (projectile.range < 0)
            _registry.remove(entity);
        });
    }
};

#endif /* !REMOVEOUTOFBOUNDPROJECTILESSYSTEM_HPP_ */
