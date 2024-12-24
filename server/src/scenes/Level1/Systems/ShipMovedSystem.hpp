/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovedSystem
*/

#ifndef SHIPMOVEDSYSTEM_HPP_
#define SHIPMOVEDSYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "NetworkAgent.hpp"
#include "ISceneManager.hpp"
#include "../Events/PacketInformations.hpp"

class ShipMovedSystem final : public ASystem
{
public:
    ShipMovedSystem(Registry &registry, ISceneManager &_manager) : ASystem(registry, "ShipMovedSystem"), _manager(_manager) {}

    void execute(const PacketInformations &event, const UserInput &input);

private:
    ISceneManager &_manager;
};

#endif /* !SHIPMOVEDSYSTEM_HPP_ */
