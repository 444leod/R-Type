/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"

#include "../../../Systems/RemoveClientSystem.hpp"
#include "../Systems/ShipMovementSystem.hpp"
#include "../Systems/ShipShotSystem.hpp"

#include "Structures/UserInput.hpp"

namespace level1
{

void handleDisconnect(ecs::Registry& registry, RestrictedSceneManager&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& src, ntw::UDPPacket&)
{
    static RemoveClientSystem removeClientSystem(registry);

    removeClientSystem.execute(src, net);
}

void handleUserInput(ecs::Registry& registry, RestrictedSceneManager&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& source, ntw::UDPPacket& packet)
{
    UserInput input{};
    packet >> input;

    if (input.key >= sf::Keyboard::Left && input.key <= sf::Keyboard::Down)
    {
        static ShipMovementSystem shipMovementSystem(registry, net);
        shipMovementSystem.execute(source, input);
    }
    else if (input.key == sf::Keyboard::Space)
    {
        static ShipShotSystem shipShotSystem(registry, net);
        shipShotSystem.execute(source);
    }
}

PacketHandlerSceneModule::PacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager, const std::shared_ptr<ANetworkSceneModule>& net) : APacketHandlerSceneModule(scene, registry, sceneManager, net)
{
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
    this->setHandler(PACKET_TYPE::USER_INPUT, handleUserInput);

}

}