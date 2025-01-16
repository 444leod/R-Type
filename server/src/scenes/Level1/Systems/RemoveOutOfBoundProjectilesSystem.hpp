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

class RemoveOutOfBoundProjectilesSystem final : public AUpdateSystem
{
public:
    RemoveOutOfBoundProjectilesSystem(ecs::Registry &registry) : AUpdateSystem(registry, "RemoveOutOfBoundProjectilesSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Projectile, Transform>().each([&](const Entity& entity, const Projectile&, const Transform& transform) {
            if (transform.x > SCREEN_WIDTH * SCALE) {
                _registry.remove(entity);
            }
        });
    }
};

#endif /* !REMOVEOUTOFBOUNDPROJECTILESSYSTEM_HPP_ */
