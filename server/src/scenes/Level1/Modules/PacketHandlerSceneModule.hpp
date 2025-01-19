/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#ifndef LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP
#define LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP

#include "PremadeModules/Network/ANetworkSceneModule.hpp"

namespace level1
{

class PacketHandlerSceneModule final : public APacketHandlerSceneModule
{
  public:
    explicit PacketHandlerSceneModule(engine::AScene& scene, const std::shared_ptr<ANetworkSceneModule>& net);
    ~PacketHandlerSceneModule() override = default;
};

} // namespace level1

#endif // LEVEL1_PACKET_HANDLER_SCENE_MODULE_HPP
