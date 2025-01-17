/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovementSystem
*/

#ifndef SHIP_MOVEMENT_SYSTEM_HPP_
#define SHIP_MOVEMENT_SYSTEM_HPP_

#include "PremadeSystems/Abstracts/ASystem.hpp"

#include "PremadeModules/Network/ANetworkSceneModule.hpp"

#include "Structures/UserInput.hpp"

class ShipMovementSystem final : public ASystem
{
public:
    explicit ShipMovementSystem(const std::shared_ptr<ANetworkSceneModule>& net) : ASystem("ShipMovementSystem"), _net(net) {}

    void execute(const asio::ip::udp::endpoint &source, const UserInput& input) const;

private:
    const std::shared_ptr<ANetworkSceneModule>& _net;
};

#endif /* !SHIP_MOVEMENT_SYSTEM_HPP_ */
