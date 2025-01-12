/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MovementSystem
*/

#ifndef MOVEMENTSYSTEM_HPP_
#define MOVEMENTSYSTEM_HPP_

#include "BaseComponents.hpp"
#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include <config.h>

class MovementSystem final : public AUpdateSystem {
  public:
    MovementSystem(Registry& registry, const std::string name = "MovementSystem") : AUpdateSystem(registry, name) {}

    void execute(const double deltaTime) override {
        _registry.view<Transform, Velocity>().each([&](const Entity& entity, Transform& transform, const Velocity& velocity) {
            transform.x += static_cast<float>((velocity.x * SCALE) * deltaTime);
            transform.y += static_cast<float>((velocity.y * SCALE) * deltaTime);
        });
    }
};

#endif /* !MOVEMENTSYSTEM_HPP_ */
