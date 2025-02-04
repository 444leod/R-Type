/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** APacketHandlerSceneModule.hpp
*/

#ifndef A_PACKET_HANDLER_SCENE_MODULE_HPP
#define A_PACKET_HANDLER_SCENE_MODULE_HPP

#include <ECS/Registry.hpp>

#include <Engine/Modules/ASceneModule.hpp>
#include <Engine/RestrictedSceneManager.hpp>

#include <Engine/RestrictedGame.hpp>
#include <Network/UDPPacket.hpp>

#include "PacketTypes.hpp"

#include <asio.hpp>

class ANetworkSceneModule;

class APacketHandlerSceneModule : public engine::ASceneModule
{
  public:
    using PacketHandler = std::function<void(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint&, ntw::UDPPacket&)>;

  protected:
    explicit APacketHandlerSceneModule(engine::AScene& scene, const std::shared_ptr<ANetworkSceneModule>& net) : ASceneModule(scene), _registry(engine::RestrictedGame::instance().registry()), _net(net) {}
    ~APacketHandlerSceneModule() override = default;

  public:
    void handlePacket(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
    {
        PACKET_TYPE type;
        packet >> type;

        if (!this->_packetHandlers.contains(static_cast<std::size_t>(type)))
        {
          std::cout << "not handled..." << std::endl;
            return;
        }
        this->_packetHandlers.at(static_cast<std::size_t>(type))(_registry, _net, src, packet);
    }

    void setHandler(const PACKET_TYPE& type, PacketHandler handler) { this->_packetHandlers.set(static_cast<std::size_t>(type), std::move(handler)); }

    void setHandlers(const std::map<PACKET_TYPE, PacketHandler>& handlers)
    {
        for (const auto& [type, handler] : handlers)
        {
            this->setHandler(type, handler);
        }
    }

  protected:
    std::shared_ptr<ANetworkSceneModule> _net;
    ecs::Registry& _registry;
    ecs::SparseSet<PacketHandler> _packetHandlers;
};

#endif // A_PACKET_HANDLER_SCENE_MODULE_HPP
