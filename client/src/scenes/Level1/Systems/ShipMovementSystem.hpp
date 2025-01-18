/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovementSystem
*/

#ifndef SHIP_MOVEMENT_SYSTEM_HPP_
#define SHIP_MOVEMENT_SYSTEM_HPP_

#include <Engine/Systems/ASystem.hpp>

#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Ship.hpp"

class ShipMovementSystem final : public engine::ASystem
{
  public:
    explicit ShipMovementSystem() : ASystem("ShipMovementSystem") {}

    void execute(const ecs::Entity& entity_id, const Velocity& velocity, const Transform& position) const
    {
        for (auto& [entity, ship, vel, pos] : _registry.view<Ship, Velocity, Transform>().each())
        {
            if (ship.id != entity_id)
                continue;
            vel = velocity;
            pos = position;
            return;
        }
    }
};

#endif /* !SHIP_MOVEMENT_SYSTEM_HPP_ */
