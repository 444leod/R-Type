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
#include "../Events/PacketInformations.hpp"

class ShipMovementSystem final : public ASystem
{
public:
    explicit ShipMovementSystem(ecs::Registry &registry) : ASystem(registry, "ShipMovementSystem") {}

    void execute(const PacketInformations &event) const {
        std::uint32_t id;
        Velocity velocity{};
        Transform position{};
        event.packet >> id >> velocity >> position;
        for (auto &[entity, ship, vel, pos] : _registry.view<Ship, Velocity, Transform>().each())
        {
            if (ship.id != id)
                continue;
            vel = velocity;
            pos = position;
            break;
        }
    }
};

#endif /* !SHIPMOVEMENTSYSTEM_HPP_ */
