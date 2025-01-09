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
#include "engine/RestrictedSceneManager.hpp"
#include "engine/RestrictedGame.hpp"

#include "NetworkModules/ANetworkSceneModule.hpp"
#include "PacketTypes.hpp"

class ANetworkGameModule;

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

        if (packetHandlerSceneModule == nullptr)
            _packetHandler = &ANetworkGameModule::_doNothing;
        else
            _packetHandler = &ANetworkGameModule::_handlePacket;
        _packetHandlerSceneModule = packetHandlerSceneModule;
    }

    void update() override
    {
    }

    void addClient(const ntw::ClientInformation& endpoint)
    {
        this->_clients.push_back(endpoint);
    }

    void removeClient(const std::uint32_t& id)
    {
        const auto it = std::ranges::find_if(this->_clients, [id](const ntw::ClientInformation& client) {
            return client.id == id;
        });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
    }

    void removeClient(const asio::ip::udp::endpoint& endpoint)
    {
        const auto it = std::ranges::find_if(this->_clients, [endpoint](const ntw::ClientInformation& client) {
            return client.endpoint == endpoint;
        });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
    }

    void removeClient(const std::string& name)
    {
        const auto it = std::ranges::find_if(this->_clients, [name](const ntw::ClientInformation& client) {
            return client.name == name;
        });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
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

    void queuePacket(const asio::ip::udp::endpoint& endpoint, const ntw::UDPPacket& packet)
    {
        this->_send(endpoint, packet);
    }

    void queuePacket(ntw::UDPPacket& packet)
    {
        for (const auto& client : this->_clients) {
            this->_send(client.endpoint, packet);
        }
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
        _packetHandlerSceneModule->handlePacket(src, packet);
    }

protected:
    // this is an optimization to avoid if/else in the update loop
    void (ANetworkGameModule::*_packetHandler)(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) = &ANetworkGameModule::_doNothing;


    std::vector<ntw::ClientInformation> _clients;

    std::shared_ptr<APacketHandlerSceneModule> _packetHandlerSceneModule;

};

#endif //A_NETWORK_GAME_MODULE_HPP