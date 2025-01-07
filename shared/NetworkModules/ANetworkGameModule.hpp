/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ANetworkGameModule.hpp
*/

#ifndef A_NETWORK_GAME_MODULE_HPP
#define A_NETWORK_GAME_MODULE_HPP

#include <utility>

#include "engine/modules/AGameModule.hpp"
#include "engine/modules/ASceneModule.hpp"
#include "ecs/Registry.hpp"
#include "network/NetworkAgent.hpp"
#include "PacketTypes.hpp"
#include "engine/RestrictedSceneManager.hpp"

#include "engine/RestrictedGame.hpp"

class ANetworkGameModule;

class APacketHandlerSceneModule : public ASceneModule
{
protected:
    explicit APacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager) : ASceneModule(scene), _registry(registry), _sceneManager(sceneManager) {}
    ~APacketHandlerSceneModule() override = default;

public:
    void handlePacket(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
    {
        PACKET_TYPE type;
        packet >> type;

        if (!this->_packetHandlers.contains(static_cast<std::size_t>(type))  )
            return;
        this->_packetHandlers.at(static_cast<std::size_t>(type))(_registry, _sceneManager, src, packet);
    }

    void setHandler(const PACKET_TYPE& type, std::function<void(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint&, ntw::UDPPacket&)> handler)
    {
        this->_packetHandlers.set(static_cast<std::size_t>(type), std::move(handler));
    }

    void setHandlers(const std::map<PACKET_TYPE, std::function<void(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint&, ntw::UDPPacket&)>>& handlers)
    {
        for (const auto& [type, handler] : handlers) {
            this->setHandler(type, handler);
        }
    }

protected:
    ecs::Registry& _registry;
    RestrictedSceneManager& _sceneManager;
    ecs::SparseSet<std::function<void(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint&, ntw::UDPPacket&)>> _packetHandlers;
};

class ANetworkGameModule : public AGameModule, public ntw::NetworkAgent {
public:
    explicit ANetworkGameModule(game::RestrictedGame& game, const std::uint32_t& port = 0) : AGameModule(game), NetworkAgent(port) {}
    ~ANetworkGameModule() override = default;

    void start() override
    {
            this->_packetHandler = &ANetworkGameModule::_handlePacket;
    }

    void stop() override
    {
            this->_packetHandler = &ANetworkGameModule::_doNothing;
    }

    void refresh(AScene& scene) override
    {
        const auto packetHandlerSceneModule = scene.getModule<APacketHandlerSceneModule>();

        _packetHandlerSceneModule = packetHandlerSceneModule;
    }

    void update() override
    {
        this->_handleStoredPackets();
        this->_sendQueuedPackets();
    }

    void addClient(const ntw::ClientInformation& endpoint)
    {
        this->_clients.push_back(endpoint);
    }

    void queuePacket(const std::uint32_t& clientId, ntw::UDPPacket& packet)
    {
        const auto it = std::ranges::find_if(this->_clients, [clientId](const ntw::ClientInformation& client) {
            return client.id == clientId;
        });

        if (it == this->_clients.end())
            return;
        this->queuePacket(it->endpoint, packet);
    }

    void queuePacket(const std::string& name, ntw::UDPPacket& packet)
    {
        const auto it = std::ranges::find_if(this->_clients, [name](const ntw::ClientInformation& client) {
            return client.name == name;
        });

        if (it == this->_clients.end())
            return;
        this->queuePacket(it->endpoint, packet);
    }

    void queuePacket(const asio::ip::udp::endpoint& endpoint, ntw::UDPPacket& packet)
    {
        this->_toSendPackets.emplace_back(endpoint, packet);
    }

    void queuePacket(ntw::UDPPacket& packet)
    {
        this->_toSendGlobalPackets.emplace_back(packet);
    }

    [[nodiscard]] std::vector<ntw::ClientInformation> clients() const
    {
        return this->_clients;
    }

protected:
    void _onPacketReceived(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) override
    {
        (this->*_packetHandler)(src, packet);
    }

    void _doNothing(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
    {
        (void)src;
        (void)packet;
    }

    void _handlePacket(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
    {
        _receivedPackets.emplace_back(src, packet);
    }

    virtual void _sendQueuedPackets()
    {
        for (auto& packet : this->_toSendGlobalPackets) {
            for (const auto& client : this->_clients) {
                this->_send(client.endpoint, packet);
            }
        }

        for (auto& [endpoint, packet] : this->_toSendPackets) {
            this->_send(endpoint, packet);
        }

        this->_toSendGlobalPackets.clear();
        this->_toSendPackets.clear();
    }

    virtual void _handleStoredPackets()
    {
        if (_packetHandlerSceneModule == nullptr) {
            _receivedPackets.clear();
            return;
        }

        for (auto& [src, packet] : _receivedPackets) {
            _packetHandlerSceneModule->handlePacket(src, packet);
        }
        _receivedPackets.clear();
    }


protected:
    // this is an optimization to avoid if/else in the update loop
    void (ANetworkGameModule::*_packetHandler)(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) = &ANetworkGameModule::_doNothing;

    std::vector<ntw::ClientInformation> _clients;
    std::vector<std::pair<const asio::ip::udp::endpoint, ntw::UDPPacket>> _receivedPackets;
    std::vector<ntw::UDPPacket> _toSendGlobalPackets;
    std::vector<std::pair<const asio::ip::udp::endpoint, ntw::UDPPacket>> _toSendPackets;

    std::shared_ptr<APacketHandlerSceneModule> _packetHandlerSceneModule;

};

#endif //A_NETWORK_GAME_MODULE_HPP