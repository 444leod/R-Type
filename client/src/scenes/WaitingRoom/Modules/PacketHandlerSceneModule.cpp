/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"

#include "PremadeComponents/Tags/Self.hpp"

#include "SharedComponents/Client.hpp"

namespace waiting_room {

void handleGameStart(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket&) {
    std::cout << "start!" << std::endl;
    engine::RestrictedGame::instance().scenes().load("game");
}

void handleDisconnect(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket&) { engine::RestrictedGame::instance().stop(); }

void handleAuthenticated(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) {
    std::uint32_t id;
    std::string name;
    packet >> id >> name;
    std::cout << "Authenticated: " << name << " (" << id << ")" << std::endl;

    const auto entity = registry.create();
    registry.addComponent(entity, Client{ntw::ClientInformation(src, id, name)});
    registry.addComponent(entity, Self{});
}

void handlerNewClient(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) {
    std::uint32_t id;
    std::string name;
    packet >> id >> name;
    std::cout << "New client: " << name << " (" << id << ")" << std::endl;

    const auto entity = registry.create();
    registry.addComponent(entity, Client{ntw::ClientInformation(src, id, name)});
}

void handleMessage(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) {
    std::uint32_t id;
    std::string message;
    packet >> id >> message;
    std::cout << "Message from " << id << ": " << message << std::endl;
}

void handleClientDisconnected(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) {
    std::uint32_t id;
    packet >> id;
    std::cout << "Client disconnected: " << id << std::endl;

    for (auto& [entity, client] : registry.view<Client>().each()) {
        if (client.info.id == id) {
            registry.remove(entity);
            break;
        }
    }
}

PacketHandlerSceneModule::PacketHandlerSceneModule(engine::AScene& scene, const std::shared_ptr<ANetworkSceneModule>& net) : APacketHandlerSceneModule(scene, net) {
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
    this->setHandler(PACKET_TYPE::AUTHENTICATED, handleAuthenticated);
    this->setHandler(PACKET_TYPE::NEW_CLIENT, handlerNewClient);

    this->setHandler(PACKET_TYPE::MESSAGE, handleMessage);

    this->setHandler(PACKET_TYPE::START, handleGameStart);

    this->setHandler(PACKET_TYPE::CLIENT_DISCONNECTED, handleClientDisconnected);
}

} // namespace waiting_room