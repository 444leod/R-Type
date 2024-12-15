/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Client
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "NetworkAgent.hpp"

#pragma once

class Client: public NetworkAgent
{
public:
    Client(asio::io_context& ctx): NetworkAgent(ctx) {}
    ~Client() = default;

    void run(const std::string& host, std::uint32_t port)
    {
        const auto addr = asio::ip::address::from_string(host);
        this->_server = asio::ip::udp::endpoint(addr, port);
        this->_send(this->_server, "CONNECT");

        this->_window.create(sf::VideoMode::getDesktopMode(), "R-Type");
        this->_window.setFramerateLimit(60);
    }

    void stop()
    {
        this->_stop();
        this->_window.close();
    }

    sf::RenderWindow &window() noexcept { return this->_window; }
    bool running() const noexcept { return this->_window.isOpen(); }

    void sendMessage(const std::string& msg)
    {
        this->_send(this->_server, msg);
    }

private:
    virtual void _onPacketReceived(const asio::ip::udp::endpoint& src, const UDPPacket& packet)
    {
        std::string msg(packet.payload.begin(), packet.payload.end());
        std::cout << "Received: " << msg << " (seq: " << packet.sequence_number
                  << ", ack: " << packet.ack_number << ")" << std::endl;
    }

protected:
private:
    asio::ip::udp::endpoint _server;
    bool _running;
    sf::RenderWindow _window;
};
