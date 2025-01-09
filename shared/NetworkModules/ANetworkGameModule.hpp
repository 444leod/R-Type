/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ANetworkGameModule.hpp
*/

#ifndef A_NETWORK_GAME_MODULE_HPP
#define A_NETWORK_GAME_MODULE_HPP

#include "engine/modules/AGameModule.hpp"
#include "ecs/Registry.hpp"
#include "network/NetworkAgent.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "engine/RestrictedGame.hpp"
#include "APacketHandlerSceneModule.hpp"

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

    void queuePacket(const std::string& name, const ntw::UDPPacket& packet)
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