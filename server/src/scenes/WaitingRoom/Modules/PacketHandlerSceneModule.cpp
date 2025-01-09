/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"

#include "../../../Systems/RemoveClientSystem.hpp"

namespace waiting_room
{

void handleDisconnect(ecs::Registry& registry, RestrictedSceneManager&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& src, ntw::UDPPacket&)
{
    static RemoveClientSystem removeClientSystem(registry);

    removeClientSystem.execute(src, net);
}

void handleConnect(ecs::Registry& registry, RestrictedSceneManager&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    auto clients = net->clients();

    const auto client = std::ranges::find_if(clients, [&src](const auto& actualClient) {
        return actualClient.endpoint == src;
    });

    if (client != clients.end())
        return;

    std::string name;
    packet >> name;
    std::cout << "Client connected: " << name << std::endl;

    const ntw::ClientInformation clientInfo(src, name);

    ntw::UDPPacket newClientPacket;
    newClientPacket << PACKET_TYPE::NEW_CLIENT;
    newClientPacket << clientInfo.id << clientInfo.name;

    net->queuePacket(newClientPacket); // the order is important : the "new client" is sent to the clients except the new one

    ntw::UDPPacket response;
    response << PACKET_TYPE::AUTHENTICATED;
    response << clientInfo.id << clientInfo.name;

    net->addClient(clientInfo);
    net->queuePacket(src, response);

    const auto newClient = registry.create();
    registry.addComponent(newClient, Client{ clientInfo });
}

PacketHandlerSceneModule::PacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager, const std::shared_ptr<ANetworkSceneModule>& net) : APacketHandlerSceneModule(scene, registry, sceneManager, net)
{
    this->setHandler(PACKET_TYPE::CONNECT, handleConnect);
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
}

}