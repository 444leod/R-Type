/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#ifndef LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP
#define LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP

#include "NetworkModules/ANetworkGameModule.hpp"

namespace waiting_room
{

class PacketHandlerSceneModule final : public APacketHandlerSceneModule
{
public:
    explicit PacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager);
    ~PacketHandlerSceneModule() override = default;
};

}

#endif //LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP