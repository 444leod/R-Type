/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovementSystem
*/

#ifndef SHIP_MOVEMENT_SYSTEM_HPP_
#define SHIP_MOVEMENT_SYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "NetworkModules/ANetworkSceneModule.hpp"

#include "Structures/UserInput.hpp"

class ShipMovementSystem final : public ASystem
{
public:
    ShipMovementSystem(ecs::Registry &registry, const std::shared_ptr<ANetworkSceneModule>& net) : ASystem(registry, "ShipMovementSystem"), _net(net) {}

    void execute(const asio::ip::udp::endpoint &source, const UserInput& input) const;

private:
    const std::shared_ptr<ANetworkSceneModule>& _net;
};

#endif /* !SHIP_MOVEMENT_SYSTEM_HPP_ */
