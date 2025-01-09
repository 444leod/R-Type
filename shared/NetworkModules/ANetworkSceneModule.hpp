/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ANetworkSceneModule.hpp
*/

#ifndef A_NETWORK_SCENE_MODULE_HPP
#define A_NETWORK_SCENE_MODULE_HPP

#include "engine/modules/ASceneModule.hpp"
#include "ANetworkGameModule.hpp" // Include full definition of ANetworkGameModule

class ANetworkSceneModule : public ASceneModule {
public:
    explicit ANetworkSceneModule(AScene& scene, ANetworkGameModule& networkGameModule) : ASceneModule(scene), _networkGameModule(networkGameModule) {}
    ~ANetworkSceneModule() override = default;

    void stop() const
    {
        this->_networkGameModule.stop();
    }

    void queuePacket(const asio::ip::udp::endpoint& dest, ntw::UDPPacket& packet) const
    {
        this->_networkGameModule.queuePacket(dest, packet);
    }

    void queuePacket(const std::uint32_t& clientId, ntw::UDPPacket& packet) const
    {
        this->_networkGameModule.queuePacket(clientId, packet);
    }

    void queuePacket(const std::string& name, ntw::UDPPacket& packet) const
    {
            this->_networkGameModule.queuePacket(name, packet);
    }

    void queuePacket(ntw::UDPPacket& packet) const
    {
            this->_networkGameModule.queuePacket(packet);
    }

    void addClient(const ntw::ClientInformation& endpoint) const
    {
        this->_networkGameModule.addClient(endpoint);
    }

    void removeClient(const std::uint32_t& id) const
    {
        this->_networkGameModule.removeClient(id);
    }

    void removeClient(const asio::ip::udp::endpoint& endpoint) const
    {
        this->_networkGameModule.removeClient(endpoint);
    }

    void removeClient(const std::string& name) const
    {
        this->_networkGameModule.removeClient(name);
    }

    [[nodiscard]] std::vector<ntw::ClientInformation> clients() const
    {
        return this->_networkGameModule.clients();
    }

protected:
    ANetworkGameModule& _networkGameModule;
};

#endif //A_NETWORK_SCENE_MODULE_HPP
