/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovedSystem
*/

#ifndef SHIPMOVEDSYSTEM_HPP_
#define SHIPMOVEDSYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "network/NetworkAgent.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "../Events/PacketInformation.hpp"
#include "../../UserInput.hpp"

class ShipMovedSystem final : public ASystem
{
public:
    ShipMovedSystem(ecs::Registry &registry, RestrictedSceneManager &_manager) : ASystem(registry, "ShipMovedSystem"), _manager(_manager) {}

    void execute(const PacketInformation &event, const UserInput &input)const;

private:
    RestrictedSceneManager &_manager;
};

#endif /* !SHIPMOVEDSYSTEM_HPP_ */
