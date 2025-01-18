/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#ifndef WAITING_ROOM_PACKET_HANDLER_SCENE_MODULE_HPP
#define WAITING_ROOM_PACKET_HANDLER_SCENE_MODULE_HPP

#include "PremadeModules/Network/ANetworkGameModule.hpp"

namespace waiting_room {

class PacketHandlerSceneModule final : public APacketHandlerSceneModule {
  public:
    explicit PacketHandlerSceneModule(engine::AScene& scene, const std::shared_ptr<ANetworkSceneModule>& net);
    ~PacketHandlerSceneModule() override = default;

  private:
};

} // namespace waiting_room

#endif // WAITING_ROOM_PACKET_HANDLER_SCENE_MODULE_HPP