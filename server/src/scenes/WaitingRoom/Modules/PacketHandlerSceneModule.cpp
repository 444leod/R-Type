/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"

#include "SharedSystems/RemoveClientSystem.hpp"

namespace waiting_room
{

void handleDisconnect(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& src, ntw::UDPPacket&)
{
    static RemoveClientSystem removeClientSystem{};

    removeClientSystem.execute(src, net);
}

void handleConnect(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    auto clients = net->clients();

    const auto client = std::ranges::find_if(clients, [&src](const auto& actualClient) { return actualClient.endpoint == src; });

    if (client != clients.end())
        return;

    std::string name;
    packet >> name;
    std::cout << "Client connected: " << name << std::endl;

    const ntw::ClientInformation clientInfo(src, name);

    ntw::UDPPacket newClientPacket;
    newClientPacket << PACKET_TYPE::NEW_CLIENT;
    newClientPacket << clientInfo.id << clientInfo.name;

    net->sendPacket(newClientPacket); // the order is important : the "new client" is sent to the clients except the new one

    ntw::UDPPacket response;
    response << PACKET_TYPE::AUTHENTICATED;
    response << clientInfo.id << clientInfo.name;

    net->addClient(clientInfo);
    net->sendPacket(src, response);

    const auto newClient = registry.create();
    registry.addComponent(newClient, Client{clientInfo});
}

PacketHandlerSceneModule::PacketHandlerSceneModule(engine::AScene& scene, const std::shared_ptr<ANetworkSceneModule>& net) : APacketHandlerSceneModule(scene, net)
{
    this->setHandler(PACKET_TYPE::CONNECT, handleConnect);
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
}

} // namespace waiting_room