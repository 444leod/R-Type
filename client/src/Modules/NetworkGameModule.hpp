/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkGameModule.hpp
*/

#ifndef NETWORK_GAME_MODULE_HPP
#define NETWORK_GAME_MODULE_HPP

#include "NetworkModules/ANetworkGameModule.hpp"
#include "PacketTypes.hpp"

/**
* @class NetworkGameModule
*
* @brief The abstract class is enough for the server to handle the network part of the game
*/
class NetworkGameModule final : public ANetworkGameModule {
public:
    explicit NetworkGameModule(game::RestrictedGame& game) : ANetworkGameModule(game) {

        std::cout << "Ip of the host (enter for localhost): " << std::flush;
        std::string ip;
        std::getline(std::cin, ip);
        if (ip.empty())
            ip = "127.0.0.1";

        std::cout << "Port of the host (enter for 25565): " << std::flush;
        std::uint32_t port;
        std::string port_str;
        std::getline(std::cin, port_str);
        if (port_str.empty())
            port = 25565;
        else
            port = std::stoi(port_str);

        const auto addr = asio::ip::address::from_string(ip);
        const auto server = asio::ip::udp::endpoint(addr, port);

        std::cout << "Player name: " << std::flush;
        std::string name;
        std::getline(std::cin, name);

        if (name.empty())
            name = "Player";

        ntw::UDPPacket packet;

        packet << PACKET_TYPE::CONNECT;
        packet << name;
        this->_clients.emplace_back(server, "Server");
        this->sendPacket(server, packet);
    }
    ~NetworkGameModule() override = default;

private:

};

#endif //NETWORK_GAME_MODULE_HPP
