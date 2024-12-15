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
#include "NetworkAgent.hpp"

#define TARGET_TICKRATE 30

/**
 * @brief This is temporary, for the example of how packets work.
 */
const std::map<PACKET_TYPE, std::function<void(UDPPacket& packet)>> packet_handlers = {
    {
        PACKET_TYPE::CONNECT, [](UDPPacket& packet)
        {
            std::cout << "Received CONNECT packet." << std::endl;
            std::string connect_message;
            packet >> connect_message;
            std::cout << "Message: " << connect_message << std::endl;
        }
    },
    {
        PACKET_TYPE::DISCONNECT, [](UDPPacket& packet)
        {
            std::cout << "Received DISCONNECT packet." << std::endl;
        }
    },
    {
        PACKET_TYPE::MESSAGE, [](UDPPacket& packet)
        {
            std::cout << "Received MESSAGE packet." << std::endl;

            std::string message;
            packet >> message;
            std::cout << "Message: " << message << std::endl;
        }
    },
    {
        PACKET_TYPE::POSITION, [](UDPPacket& packet)
        {
            std::cout << "Received POSITION packet." << std::endl;

            Position pos{};
            packet >> pos;
            std::cout << "Position: (" << pos.x << ", " << pos.y << ")" << std::endl;
        }
    }
};

class Server: public NetworkAgent
{
public:
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
        while (this->_running) {
            const auto tick_start = std::chrono::high_resolution_clock::now();
            // v v Server-loop here v v //
            // std::cout << "Tick: " << this->_tick++ << std::endl;
            if (this->_tick && this->_tick % 100 == 0) {
                std::stringstream ss;
                ss << "You were here at tick " << this->_tick << "..." << std::endl;
                this->_broadcast(ss.str());
            }
            // ^ ^ Server loop here ^ ^ //
            std::this_thread::sleep_until(tick_start + std::chrono::milliseconds(target_tick_delta));
        }
    }

private:
    void _onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) override
    {
        const auto payload = packet.payload;

        std::cout << "Received: " << payload << " (seq: " << packet.sequence_number
            << ", ack: " << packet.ack_number << ")" << std::endl;

        /**
         * @brief This code is temporary!!!
        */
        PACKET_TYPE packet_type{};
        packet >> packet_type;
         if (packet_handlers.contains(packet_type))
            packet_handlers.at(packet_type)(packet);

        auto it = std::find(this->_clients.begin(), this->_clients.end(), src);
        if (it == this->_clients.end())
            this->_clients.push_back(src);
    }

    void _broadcast(const std::string& message)
    {
        UDPPacket packet;
        packet << message;
        for (auto& client : this->_clients) {
            this->_send(client, packet);
        }
    }

private:
    std::vector<asio::ip::udp::endpoint> _clients;
    bool _running = false;
    std::uint64_t _tick = 0;
};
