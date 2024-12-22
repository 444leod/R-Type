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
#include <config.h>

class RemoveOutOfBoundProjectilesSystem final : public AUpdateSystem
{
public:
    RemoveOutOfBoundProjectilesSystem(Registry &registry) : AUpdateSystem(registry) {}

    void execute(const double deltaTime) override {
        _registry.view<Projectile, Transform>().each([&](const Entity& entity, const Projectile&, const Transform& transform) {
            if (transform.x > SCREEN_WIDTH * SCALE) {
                _registry.remove(entity);
            }
        });
    }

private:
    const std::string _name = "RemoveOutOfBoundProjectilesSystem";
};

#endif /* !REMOVEOUTOFBOUNDPROJECTILESSYSTEM_HPP_ */
