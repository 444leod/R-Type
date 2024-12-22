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
#include <config.h>

class MovementSystem final : public AUpdateSystem
{
public:
    MovementSystem(Registry &registry) : AUpdateSystem(registry) {}

    void execute(const double deltaTime, const sf::RenderWindow &window) override {
        _registry.view<Transform, Velocity>().each([&](const Entity& entity, Transform& transform, const Velocity& velocity) {
            transform.x += static_cast<float>((velocity.x * SCALE) * deltaTime);
            transform.y += static_cast<float>((velocity.y * SCALE) * deltaTime);
        });
    }

private:
    const std::string _name = "MovementSystem";
};

#endif /* !MOVEMENTSYSTEM_HPP_ */
