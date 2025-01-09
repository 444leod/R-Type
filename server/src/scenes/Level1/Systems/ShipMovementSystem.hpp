/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovementSystem
*/

#ifndef SHIP_MOVEMENT_SYSTEM_HPP_
#define SHIP_MOVEMENT_SYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "network/NetworkAgent.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "../Events/PacketInformation.hpp"
#include "../../UserInput.hpp"

class ShipMovementSystem final : public ASystem
{
public:
    ShipMovementSystem(ecs::Registry &registry, RestrictedSceneManager &_manager) : ASystem(registry, "ShipMovementSystem"), _manager(_manager) {}

    void execute(const PacketInformation &event, const UserInput &input) const;

private:
    RestrictedSceneManager &_manager;
};

#endif /* !SHIP_MOVEMENT_SYSTEM_HPP_ */
