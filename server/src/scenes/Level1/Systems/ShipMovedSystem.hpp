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
#include "engine/ISceneManager.hpp"
#include "../Events/PacketInformations.hpp"
#include "../../UserInput.hpp"

class ShipMovedSystem final : public ASystem
{
public:
    ShipMovedSystem(ecs::Registry &registry, ISceneManager &_manager) : ASystem(registry, "ShipMovedSystem"), _manager(_manager) {}

    void execute(const PacketInformations &event, const UserInput &input)const;

private:
    ISceneManager &_manager;
};

#endif /* !SHIPMOVEDSYSTEM_HPP_ */
