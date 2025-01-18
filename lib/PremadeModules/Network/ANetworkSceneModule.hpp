/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ANetworkSceneModule.hpp
*/

#ifndef A_NETWORK_SCENE_MODULE_HPP
#define A_NETWORK_SCENE_MODULE_HPP

#include "ANetworkGameModule.hpp" // Include full definition of ANetworkGameModule
#include "Engine/Modules/ASceneModule.hpp"

class ANetworkSceneModule : public engine::ASceneModule {
  public:
    explicit ANetworkSceneModule(engine::AScene& scene, ANetworkGameModule& networkGameModule) : ASceneModule(scene), _networkGameModule(networkGameModule) {}
    ~ANetworkSceneModule() override = default;

    void stop() const { this->_networkGameModule.stop(); }

    void sendPacket(const asio::ip::udp::endpoint& dest, const ntw::UDPPacket& packet) const { this->_networkGameModule.sendPacket(dest, packet); }

    void sendPacket(const std::uint32_t& clientId, ntw::UDPPacket& packet) const { this->_networkGameModule.sendPacket(clientId, packet); }

    void sendPacket(const std::string& name, const ntw::UDPPacket& packet) const { this->_networkGameModule.sendPacket(name, packet); }

    void sendPacket(const ntw::UDPPacket& packet) const { this->_networkGameModule.sendPacket(packet); }

    void addClient(const ntw::ClientInformation& endpoint) const { this->_networkGameModule.addClient(endpoint); }

    void removeClient(const std::uint32_t& id) const { this->_networkGameModule.removeClient(id); }

    void removeClient(const asio::ip::udp::endpoint& endpoint) const { this->_networkGameModule.removeClient(endpoint); }

    void removeClient(const std::string& name) const { this->_networkGameModule.removeClient(name); }

    void resend() const { this->_networkGameModule.resend(); }

    [[nodiscard]] std::vector<ntw::ClientInformation> clients() const { return this->_networkGameModule.clients(); }

  protected:
    ANetworkGameModule& _networkGameModule;
};

#endif // A_NETWORK_SCENE_MODULE_HPP
