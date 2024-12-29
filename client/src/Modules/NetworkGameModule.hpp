/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkGameModule.hpp
*/

#ifndef NETWORK_GAME_MODULE_HPP
#define NETWORK_GAME_MODULE_HPP

#include "ANetworkGameModule.hpp"
#include "PacketTypes.hpp"

/**
* @class NetworkGameModule
*
* @brief The abstract class is enough for the server to handle the network part of the game
*/
class NetworkGameModule final : public ANetworkGameModule {
public:
    explicit NetworkGameModule(const std::string& ip, const std::uint32_t& port) : ANetworkGameModule() {
        const auto addr = asio::ip::address::from_string(ip);
        auto server = asio::ip::udp::endpoint(addr, port);

        ntw::UDPPacket packet;

        packet << PACKET_TYPE::CONNECT;
        this->_send(server, packet);
    }
    ~NetworkGameModule() override = default;

private:

};

#endif //NETWORK_GAME_MODULE_HPP
