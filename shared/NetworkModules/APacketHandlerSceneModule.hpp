/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** APacketHandlerSceneModule.hpp
*/

#ifndef A_PACKET_HANDLER_SCENE_MODULE_HPP
#define A_PACKET_HANDLER_SCENE_MODULE_HPP

#include "engine/modules/ASceneModule.hpp"
#include "ecs/Registry.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "network/UDPPacket.hpp"
#include "PacketTypes.hpp"
#include "engine/RestrictedGame.hpp"
#include <asio.hpp>

class ANetworkSceneModule;

class APacketHandlerSceneModule : public ASceneModule
{
protected:
    explicit APacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager, const std::shared_ptr<ANetworkSceneModule>& net) : ASceneModule(scene), _registry(registry), _sceneManager(sceneManager), _net(net) {}
    ~APacketHandlerSceneModule() override = default;

public:
    void handlePacket(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
    {
        PACKET_TYPE type;
        packet >> type;

        if (!this->_packetHandlers.contains(static_cast<std::size_t>(type))  )
            return;
        this->_packetHandlers.at(static_cast<std::size_t>(type))(_registry, _sceneManager, _net, src, packet);
    }

    void setHandler(const PACKET_TYPE& type, std::function<void(ecs::Registry& registry, RestrictedSceneManager& manager, std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint&, ntw::UDPPacket&)> handler)
    {
        this->_packetHandlers.set(static_cast<std::size_t>(type), std::move(handler));
    }

    void setHandlers(const std::map<PACKET_TYPE, std::function<void(ecs::Registry& registry, RestrictedSceneManager& manager, std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint&, ntw::UDPPacket&)>>& handlers)
    {
        for (const auto& [type, handler] : handlers) {
            this->setHandler(type, handler);
        }
    }

protected:
    ecs::Registry& _registry;
    RestrictedSceneManager& _sceneManager;
    std::shared_ptr<ANetworkSceneModule> _net;
    ecs::SparseSet<std::function<void(ecs::Registry& registry, RestrictedSceneManager& manager, std::shared_ptr<ANetworkSceneModule>& net, const asio::ip::udp::endpoint&, ntw::UDPPacket&)>> _packetHandlers;
};

#endif //A_PACKET_HANDLER_SCENE_MODULE_HPP
