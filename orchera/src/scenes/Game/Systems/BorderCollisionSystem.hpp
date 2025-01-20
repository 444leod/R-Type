/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** BorderCollisionSystem.hpp
*/

#ifndef BORDER_COLLISION_SYSTEM_HPP
#define BORDER_COLLISION_SYSTEM_HPP

#include <Engine/Systems/AUpdateSystem.hpp>

#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "../../../Components/Monster.hpp"
#include "../../../Components/Border.hpp"
#include "../../../Components/Player.hpp"

#include "Config.hpp"

#include <cmath>

class BorderCollisionSystem final : public engine::AUpdateSystem
{
public:
    BorderCollisionSystem() : AUpdateSystem("BorderCollisionSystem")
    {
    }

    void _execution(double deltaTime) override
    {
        const auto& borders = _registry.view<Transform, Border>().each();
        _registry.view<Monster, Transform, Velocity>().each([&](const ecs::Entity& entity, const Monster& monster, Transform &transform, const Velocity &velocity) {
            updatePosOnWalls(transform, velocity, deltaTime, borders);
        });
        _registry.view<Player, Transform, Velocity>().each([&](const ecs::Entity& entity, const Player& projectile, Transform &transform, const Velocity &velocity) {
            updatePosOnWalls(transform, velocity, deltaTime, borders);
        });

    }

private:
    void updatePosOnWalls(Transform &transform, const Velocity &velocity, const double &deltaTime, const std::vector<std::tuple<ecs::Entity, Transform&, Border&>>& borders)
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
