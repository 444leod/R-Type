/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"

namespace waiting_room
{

void handleGameStart(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    manager.load("game");
}

void handleDisconnect(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    game::RestrictedGame::instance().stop();
}

PacketHandlerSceneModule::PacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager) : APacketHandlerSceneModule(scene, registry, sceneManager) {
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
    this->setHandler(PACKET_TYPE::START, handleGameStart);
}

}