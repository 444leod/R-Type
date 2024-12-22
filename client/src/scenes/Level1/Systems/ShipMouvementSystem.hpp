/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMouvementSystem
*/

#ifndef SHIPMOUVEMENTSYSTEM_HPP_
#define SHIPMOUVEMENTSYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"
#include <config.h>
#include "../Events/PacketInformations.hpp"

class ShipMouvementSystem final : public ASystem
{
public:
    ShipMouvementSystem(Registry &registry, const std::string name = "ShipMouvementSystem") : ASystem(registry, name) {}

    void execute(const PacketInformations &event) {
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

#endif /* !SHIPMOUVEMENTSYSTEM_HPP_ */
