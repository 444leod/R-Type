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
#include "NetworkAgent.hpp"

#define TARGET_TICKRATE 30

/**
 * @brief Class representation of a game server, receives packet and can broadcast
 */
class Server: public NetworkAgent
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
        std::uint32_t target_tick_delta = 1000 / TARGET_TICKRATE;

        this->_running = true;
        std::cout << "Game is running..." << std::endl;
        while (this->_running) {
            const auto tick_start = std::chrono::high_resolution_clock::now();
            // v v Server-loop here v v //
            std::cout << "Tick: " << this->_tick++ << std::endl;
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
    /**
     * @brief Callback called when a packet was received
     * @param src The source that sent the packet
     * @param msg The message contained in the packet
     */
    virtual void _onPacketReceived(const asio::ip::udp::endpoint& src, const std::string& msg)
    {
        std::cout << "Received: " << msg << std::endl;

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
        for (auto& client : this->_clients) {
            this->_send(client, message);
        }
    }

private:
    std::vector<asio::ip::udp::endpoint> _clients;
    bool _running = false;
    std::uint64_t _tick = 0;
};
