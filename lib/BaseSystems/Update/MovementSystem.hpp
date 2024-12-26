/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MovementSystem
*/

#ifndef MOVEMENTSYSTEM_HPP_
#define MOVEMENTSYSTEM_HPP_

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"
#include "Config.hpp"


class MovementSystem final : public AUpdateSystem
{
public:
    explicit MovementSystem(ecs::Registry &registry) : AUpdateSystem(registry, "MovementSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Transform, Velocity>().each([&](const Entity& entity, Transform& transform, const Velocity& velocity) {
            transform.x += static_cast<float>((velocity.x * SCALE) * deltaTime);
            transform.y += static_cast<float>((velocity.y * SCALE) * deltaTime);
        });
    }
};

#endif /* !MOVEMENTSYSTEM_HPP_ */
