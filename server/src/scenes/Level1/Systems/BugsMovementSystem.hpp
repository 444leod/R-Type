/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BugsMovementSystem
*/

#ifndef MOVEMENTBUGSSYSTEM_HPP_
#define MOVEMENTBUGSSYSTEM_HPP_

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"
#include <cmath>

class BugsMovementSystem final : public AUpdateSystem
{
public:
    explicit BugsMovementSystem(ecs::Registry &registry) : AUpdateSystem(registry, "BugsMovementSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Bug, sf::Sprite, Transform>().each([&](const Entity& entity, Bug& bug, sf::Sprite& sprite, Transform& transform) {
            const auto movementFactor = std::sin(bug.clock.getElapsedTime().asSeconds() / .2);
            transform.y += movementFactor * 8;
            transform.rotation = 90 - 45 * movementFactor;
        });
    }
};

#endif /* !MOVEMENTBUGSSYSTEM_HPP_ */
