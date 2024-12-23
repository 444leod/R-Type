/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Server
*/

#pragma once

#include <asio.hpp>
#include <thread>
#include <iostream>
#include <vector>
#include <sstream>
#include <functional>
#include <map>
#include "network/NetworkAgent.hpp"

#define TARGET_TICKRATE 30

bool isInputAvailable() {
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    return select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv) > 0;
}

/**
 * @brief Class representation of a game server, receives packet and can broadcast
 */
class Server: public ntw::NetworkAgent
{
public:
    /**
     * @brief Constructor for the Server, this instantly add asio work
     * @param ctx An io_context to bind the sockets' work to
     */
    Server(asio::io_context& ctx): NetworkAgent(ctx, 25565)
    {
        std::cout << "Server started, listening on port: " << this->_port << "..." << std::endl;
    }
    ~Server() = default;

    /**
     * @brief Starts the Server's game loop
     */
    void run()
    {
        constexpr std::uint32_t target_tick_delta = 1000 / TARGET_TICKRATE;

        this->_running = true;
        std::cout << "Game is running..." << std::endl;

        std::cout << "> ";
        std::flush(std::cout);

        while (this->_running) {
            const auto tick_start = std::chrono::high_resolution_clock::now();
            // v v Server-loop here v v //

            if (isInputAvailable()) {
                std::string input;
                if (!std::getline(std::cin, input))
                {
                    if (std::cin.eof()) {
                        _command_handlers["exit"]();
                        break;
                    } else {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } else
                {
                    if (_command_handlers.contains(input))
                        _command_handlers.at(input)();
                }

                std::cout << "> ";
                std::flush(std::cout);
            }

            // ^ ^ Server loop here ^ ^ //
            std::this_thread::sleep_until(tick_start + std::chrono::milliseconds(target_tick_delta));
        }
    }

private:
    /**
     * @brief Callback called when a packet was received
     * @param src The source that sent the packet
     * @param packet The packet received
     */
    void _onPacketReceived(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) override
    {
        const auto payload = packet.payload;

        std::cout << "Received: " << payload << " (seq: " << packet.sequence_number
            << ", ack: " << packet.ack_number << ")" << std::endl;

        /**
         * @brief This code is temporary!!!
        */
//        ntw::PACKET_TYPE packet_type{};
//        packet >> packet_type;
//         if (_packet_handlers.contains(packet_type))
//            _packet_handlers.at(packet_type)(src, packet);

        auto it = std::find(this->_clients.begin(), this->_clients.end(), src);
        if (it == this->_clients.end())
            this->_clients.push_back(src);
    }

    /**
     * @brief Broadcast a message to all known client
     * @param message The message to broadcast
     */
    void _broadcast(const std::string& message)
    {
        ntw::UDPPacket packet;
        packet << message;
        for (auto& client : this->_clients) {
            this->_send(client, packet);
        }
    }

    void _broadcast(const ntw::UDPPacket& packet)
    {
        for (auto& client : this->_clients)
        {
            this->_send(client, packet);
        }
    }

private:
    std::vector<asio::ip::udp::endpoint> _clients;
    bool _running = false;
    std::uint64_t _tick = 0;

    std::map<asio::ip::udp::endpoint, std::uint32_t> _client_ids;

    std::map<std::string, std::function<void(void)>> _command_handlers = {
    { "", []() {} },
    { "exit", [&]()
    {
        this->_running = false;
        ntw::UDPPacket packet;
        packet << ntw::PACKET_TYPE::DISCONNECT;
        this->_broadcast(packet);
        std::cout << "Exiting..." << std::endl;
    } },
    { "start", [&]()
    {
        ntw::UDPPacket packet;
        packet << ntw::PACKET_TYPE::START;
        this->_broadcast(packet);
        _command_handlers["start"] = []() {};
    } },
    };
};
