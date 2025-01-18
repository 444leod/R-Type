/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ANetworkGameModule.hpp
*/

#ifndef A_NETWORK_GAME_MODULE_HPP
#define A_NETWORK_GAME_MODULE_HPP

#include <Engine/AScene.hpp>
#include <Engine/Modules/AGameModule.hpp>

#include <Network/NetworkAgent.hpp>

#include "APacketHandlerSceneModule.hpp"

class ANetworkGameModule : public engine::AGameModule, public ntw::NetworkAgent {
  public:
    explicit ANetworkGameModule(const std::uint32_t& port = 0) : AGameModule(), NetworkAgent(port) {}
    ~ANetworkGameModule() override = default;

    void start(engine::AScene& scene) override {
        const auto packetHandlerSceneModule = scene.getModule<APacketHandlerSceneModule>();

        if (packetHandlerSceneModule == nullptr) {
            _packetHandler = &ANetworkGameModule::_doNothing;
        } else {
            _packetHandler = &ANetworkGameModule::_handlePacket;
        }
        _packetHandlerSceneModule = packetHandlerSceneModule;

        for (auto& [endpoint, packet] : _storedPackets) {
            (this->*_packetHandler)(endpoint, packet);
        }
        _storedPackets.clear();
    }

    void stop() override { this->_packetHandler = &ANetworkGameModule::_doNothing; }

    void refresh(engine::AScene& scene) override {
        const auto packetHandlerSceneModule = scene.getModule<APacketHandlerSceneModule>();

        if (packetHandlerSceneModule == nullptr) {
            _packetHandler = &ANetworkGameModule::_doNothing;
        } else {
            _packetHandler = &ANetworkGameModule::_handlePacket;
        }
        _packetHandlerSceneModule = packetHandlerSceneModule;
    }

    void update() override {}

    void addClient(const ntw::ClientInformation& endpoint) { this->_clients.push_back(endpoint); }

    void removeClient(const std::uint32_t& id) {
        const auto it = std::ranges::find_if(this->_clients, [id](const ntw::ClientInformation& client) { return client.id == id; });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
    }

    void removeClient(const asio::ip::udp::endpoint& endpoint) {
        const auto it = std::ranges::find_if(this->_clients, [endpoint](const ntw::ClientInformation& client) { return client.endpoint == endpoint; });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
    }

    void removeClient(const std::string& name) {
        const auto it = std::ranges::find_if(this->_clients, [name](const ntw::ClientInformation& client) { return client.name == name; });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
    }

    void sendPacket(const std::uint32_t& clientId, ntw::UDPPacket& packet) {
        const auto it = std::ranges::find_if(this->_clients, [clientId](const ntw::ClientInformation& client) { return client.id == clientId; });

        if (it == this->_clients.end())
            return;
        _lastPacket = std::pair(it->endpoint, packet);
        this->sendPacket(it->endpoint, packet);
    }

    void sendPacket(const std::string& name, const ntw::UDPPacket& packet) {
        const auto it = std::ranges::find_if(this->_clients, [name](const ntw::ClientInformation& client) { return client.name == name; });

        if (it == this->_clients.end())
            return;
        _lastPacket = std::pair(it->endpoint, packet);
        this->sendPacket(it->endpoint, packet);
    }

    void sendPacket(const asio::ip::udp::endpoint& endpoint, const ntw::UDPPacket& packet) {
        _lastPacket = std::pair(endpoint, packet);
        this->_send(endpoint, packet);
    }

    void sendPacket(const ntw::UDPPacket& packet) {
        _lastPacket = packet;
        for (const auto& client : this->_clients) {
            this->_send(client.endpoint, packet);
        }
    }

    void resend() {
        if (!_lastPacket.has_value())
            return;

        if (std::holds_alternative<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>>(_lastPacket.value())) {
            const auto& [endpoint, packet] = std::get<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>>(_lastPacket.value());
            this->sendPacket(endpoint, packet);
        } else {
            const auto& packet = std::get<ntw::UDPPacket>(_lastPacket.value());
            this->sendPacket(packet);
        }
    }

    [[nodiscard]] std::vector<ntw::ClientInformation> clients() const { return this->_clients; }

  protected:
    void _onPacketReceived(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) override { (this->*_packetHandler)(src, packet); }

    void _storePacket(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) { _storedPackets.emplace_back(src, packet); }

    void _doNothing(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) {
        (void)src;
        (void)packet;
    }

    void _handlePacket(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) { _packetHandlerSceneModule->handlePacket(src, packet); }

  protected:
    // this is an optimization to avoid if/else in the update loop
    void (ANetworkGameModule::*_packetHandler)(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) = &ANetworkGameModule::_storePacket;

    std::vector<ntw::ClientInformation> _clients;

    std::vector<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>> _storedPackets;

    std::shared_ptr<APacketHandlerSceneModule> _packetHandlerSceneModule;

    std::optional<std::variant<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>, ntw::UDPPacket>> _lastPacket;
};

#endif // A_NETWORK_GAME_MODULE_HPP