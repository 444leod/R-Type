/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** BorderCollisionSystem.hpp
*/

#ifndef BORDER_COLLISION_SYSTEM_HPP
#define BORDER_COLLISION_SYSTEM_HPP

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"
#include "Config.hpp"

#include "../../../Components/Monster.hpp"
#include "../../../Components/Border.hpp"
#include "../../../Components/Player.hpp"

#include <cmath>

class BorderCollisionSystem final : public AUpdateSystem
{
public:
    explicit BorderCollisionSystem(ecs::Registry &registry) : AUpdateSystem(registry, "BorderCollisionSystem")
    {
    }

    void execute(const double& deltaTime)
    {
        const auto& borders = _registry.view<Transform, Border>().each();
        _registry.view<Monster, Transform, Velocity>().each([&](const Entity& entity, const Monster& monster, Transform &transform, const Velocity &velocity) {
            updatePosOnWalls(transform, velocity, deltaTime, borders);
        });
        _registry.view<Player, Transform, Velocity>().each([&](const Entity& entity, const Player& projectile, Transform &transform, const Velocity &velocity) {
            updatePosOnWalls(transform, velocity, deltaTime, borders);
        });

    }

private:
    void updatePosOnWalls(Transform &transform, const Velocity &velocity, const double &deltaTime, const std::vector<std::tuple<Entity, Transform&, Border&>>& borders)
    {
        for (const auto& [_, borderTransform, border] : borders)
        {
            if (transform.x >= borderTransform.x && transform.x <= borderTransform.x + border.width &&
            transform.y >= borderTransform.y && transform.y <= borderTransform.y + border.height)
            {
                transform.x -= velocity.x * SCALE * deltaTime;
                transform.y -= velocity.y * SCALE * deltaTime;
            }
        }
    }
};

#endif //BORDER_COLLISION_SYSTEM_HPP
