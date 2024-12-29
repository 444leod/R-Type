/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ANetworkGameModule.hpp
*/

#ifndef A_NETWORK_GAME_MODULE_HPP
#define A_NETWORK_GAME_MODULE_HPP

#include "engine/modules/IGameModule.hpp"
#include "network/NetworkAgent.hpp"

class ANetworkGameModule : public IGameModule, public ntw::NetworkAgent {
public:
    explicit ANetworkGameModule(const std::uint32_t& port = 0) : IGameModule(), NetworkAgent(port) {}
    ~ANetworkGameModule() override = default;

    void start() override
    {
            this->_packetHandler = &ANetworkGameModule::_handlePacket;
    }

    void stop() override
    {
            this->_packetHandler = &ANetworkGameModule::_doNothing;
    }

    void update(game::RestrictedGame &game) override
    {
        this->_handleStoredPackets(game);
        this->_sendQueuedPackets();
    }

    void addClient(ntw::ClientInformation& endpoint)
    {
            this->_clients.emplace_back(endpoint);
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

    virtual void _handleStoredPackets(game::RestrictedGame &game)
    {
        //    const auto* scenePacketHandler = game.scenes().current().getModule<PacketHandlerSceneModule>();

        //    if (scenePacketHandler == nullptr) {
        //        _receivedPackets.clear();
        //        return;
        //    }

        PACKET_TYPE type;
        for (auto& [src, packet] : _receivedPackets) {
            packet >> type;
            std::cout << "Got a packet from " << src << " with type " << static_cast<int>(type) << std::endl;
            // scenePacketHandler->handlePacket(src, packet);
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

};

#endif //A_NETWORK_GAME_MODULE_HPP