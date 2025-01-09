/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#ifndef LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP
#define LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP

#include "NetworkModules/ANetworkGameModule.hpp"

namespace level1
{

class PacketHandlerSceneModule final : public APacketHandlerSceneModule
{
public:
    explicit PacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager, const std::shared_ptr<ANetworkSceneModule>& net);
    ~PacketHandlerSceneModule() override = default;
};

}

#endif //LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP