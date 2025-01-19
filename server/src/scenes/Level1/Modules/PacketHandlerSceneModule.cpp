/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"

#include "../../../../../client/src/scenes/Level1/Systems/NewProjectileSystem.hpp"
#include "SharedSystems/RemoveClientSystem.hpp"

#include "../Systems/ShipMovementSystem.hpp"
#include "../Systems/ShipShotSystem.hpp"

#include "Structures/UserInput.hpp"

namespace level1
{

void handleDisconnect(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& src, ntw::UDPPacket&)
{
    static RemoveClientSystem removeClientSystem{};

    removeClientSystem.execute(src, net);
}

void handleUserInput(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& source, ntw::UDPPacket& packet)
{
    UserInput input{};
    packet >> input;

    if (input.key >= sf::Keyboard::Left && input.key <= sf::Keyboard::Down)
    {
        static ShipMovementSystem shipMovementSystem(net);
        shipMovementSystem.execute(source, input);
    }
}

void handleProjectile(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& source, ntw::UDPPacket& packet)
{
    std::uint32_t id;
    packet >> id;

    static ShipShotSystem shipShotSystem{net};
    shipShotSystem.execute(source, id);
}

PacketHandlerSceneModule::PacketHandlerSceneModule(engine::AScene& scene, const std::shared_ptr<ANetworkSceneModule>& net) : APacketHandlerSceneModule(scene, net)
{
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
    this->setHandler(PACKET_TYPE::NEW_PROJECTILE, handleProjectile);
    this->setHandler(PACKET_TYPE::USER_INPUT, handleUserInput);
}

} // namespace level1