/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"
#include "SharedSystems/RemoveClientSystem.hpp"
#include "../../../Room/RoomManager.hpp"
#include "../../Level1/Modules/PacketHandlerSceneModule.hpp"

namespace waiting_room
{

std::map<asio::ip::udp::endpoint, std::uint32_t> playerRooms;

void handleDisconnect(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& src, ntw::UDPPacket&)
{
    static RemoveClientSystem removeClientSystem{};

    if (auto it = playerRooms.find(src); it != playerRooms.end()) {
        RoomManager::instance().leaveRoom(it->second, src);
        playerRooms.erase(it);
    }

    removeClientSystem.execute(src, net);
}

void handleConnect(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
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

    net->sendPacket(newClientPacket); // the order is important : the "new client" is sent to the clients except the new one

    ntw::UDPPacket response;
    response << PACKET_TYPE::AUTHENTICATED;
    response << clientInfo.id << clientInfo.name;

    net->addClient(clientInfo);
    net->sendPacket(src, response);

    const auto newClient = registry.create();
    registry.addComponent(newClient, Client{ clientInfo });
}

void handleCreateRoom(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& source, ntw::UDPPacket& packet)
{
    std::string roomName;
    std::uint8_t maxPlayers;
    packet >> roomName >> maxPlayers;

    auto room = RoomManager::instance().createRoom(roomName, maxPlayers);
    auto clients = net->clients();
    auto clientInfo = std::find_if(clients.begin(), clients.end(),
        [&source](const auto& client) { return client.endpoint == source; });

    if (clientInfo != clients.end() && RoomManager::instance().joinRoom(room->id, *clientInfo)) {
        playerRooms[source] = room->id;
        ntw::UDPPacket response;
        response << PACKET_TYPE::ROOM_JOINED << room->id;
        net->sendPacket(source, response);
    }
}

void handleJoinRoom(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& source, ntw::UDPPacket& packet)
{
    std::uint32_t roomId;
    packet >> roomId;

    auto clients = net->clients();
    auto clientInfo = std::find_if(clients.begin(), clients.end(),
        [&source](const auto& client) { return client.endpoint == source; });

    if (clientInfo != clients.end() && RoomManager::instance().joinRoom(roomId, *clientInfo)) {
        std::cout << "Client " << clientInfo->name << " joined room " << roomId << std::endl;
        playerRooms[source] = roomId;
        ntw::UDPPacket response;
        response << PACKET_TYPE::ROOM_JOINED << roomId;
        net->sendPacket(source, response);
    } else {
        std::cout << "Client " << clientInfo->name << " failed to join room " << roomId << std::endl;
        ntw::UDPPacket response;
        response << PACKET_TYPE::ROOM_ERROR << roomId;
        net->sendPacket(source, response);
    }
}

void handleGameInput(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint& source, ntw::UDPPacket& packet)
{
    auto roomIt = playerRooms.find(source);
    if (roomIt == playerRooms.end()) return;

    auto room = RoomManager::instance().getRoom(roomIt->second);
    if (!room || !room->isStarted || !room->gameScene) return;

    if (auto packetHandler = room->gameScene->getModule<level1::PacketHandlerSceneModule>()) {
        packetHandler->handlePacket(source, packet);
    }
}

PacketHandlerSceneModule::PacketHandlerSceneModule(engine::AScene& scene, const std::shared_ptr<ANetworkSceneModule>& net) : APacketHandlerSceneModule(scene, net)
{
    this->setHandler(PACKET_TYPE::CONNECT, handleConnect);
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
    this->setHandler(PACKET_TYPE::CREATE_ROOM, handleCreateRoom);
    this->setHandler(PACKET_TYPE::JOIN_ROOM, handleJoinRoom);
}

} // namespace waiting_room
