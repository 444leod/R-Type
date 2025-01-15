/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovementSystem
*/

#ifndef SHIPMOVEMENTSYSTEM_HPP_
#define SHIPMOVEMENTSYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"

class ShipMovementSystem final : public ASystem
{
public:
    explicit ShipMovementSystem(ecs::Registry &registry) : ASystem(registry, "ShipMovementSystem") {}

    void execute(const Entity& entity_id, const Velocity& velocity, const Transform& position) const {
        for (auto &[entity, ship, vel, pos] : _registry.view<Ship, Velocity, Transform>().each())
        {
            if (ship.id != entity_id)
                continue;
            vel = velocity;
            pos = position;
            return;
        }
    }
};

#endif /* !SHIPMOVEMENTSYSTEM_HPP_ */
