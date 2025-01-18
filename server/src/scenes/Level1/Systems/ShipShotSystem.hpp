/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipShotSystem
*/

#ifndef SHIP_SHOT_SYSTEM_HPP_
#define SHIP_SHOT_SYSTEM_HPP_

#include "PremadeModules/Network/ANetworkSceneModule.hpp"

#include "PremadeSystems/Abstracts/ASystem.hpp"

class ShipShotSystem final : public ASystem
{
  public:
    explicit ShipShotSystem(const std::shared_ptr<ANetworkSceneModule>& net) : ASystem("ShipShotSystem"), _net(net) {}

    void execute(const asio::ip::udp::endpoint& source) const;

  private:
    const std::shared_ptr<ANetworkSceneModule>& _net;
};

#endif /* !SHIP_SHOT_SYSTEM_HPP_ */
