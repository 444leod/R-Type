/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#ifndef WAITING_ROOM_PACKET_HANDLER_SCENE_MODULE_HPP
#define WAITING_ROOM_PACKET_HANDLER_SCENE_MODULE_HPP

#include "NetworkModules/ANetworkGameModule.hpp"

namespace level1
{

class PacketHandlerSceneModule final : public APacketHandlerSceneModule
{
public:
    explicit PacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager);
    ~PacketHandlerSceneModule() override = default;
};

}

#endif //WAITING_ROOM_PACKET_HANDLER_SCENE_MODULE_HPP