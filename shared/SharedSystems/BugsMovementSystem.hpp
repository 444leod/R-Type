/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BugsMovementSystem
*/

#ifndef MOVEMENT_BUGS_SYSTEM_HPP_
#define MOVEMENT_BUGS_SYSTEM_HPP_

#include <Engine/Systems/AUpdateSystem.hpp>

#include "SharedComponents/Bug.hpp"

#include <cmath>

class BugsMovementSystem final : public engine::AUpdateSystem
{
  public:
    explicit BugsMovementSystem() : AUpdateSystem("BugsMovementSystem") {}

private:
    void _execution(double deltaTime) override
    {
        _registry.view<Bug, Transform>().each(
            [&](const Bug& bug, Transform& transform)
            {
                const auto movementFactor = std::sin(bug.clock.getElapsedTime().asSeconds() / .2);
                transform.y += movementFactor * 8;
                transform.rotation = 90 - 45 * movementFactor;
            });
    }
};

#endif /* !MOVEMENT_BUGS_SYSTEM_HPP_ */
