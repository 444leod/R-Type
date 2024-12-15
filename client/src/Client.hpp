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

        UDPPacket packet;
        packet << PACKET_TYPE::CONNECT;
        packet << "CONNECT";
        this->_send(this->_server, packet);

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
        UDPPacket packet;
        packet << PACKET_TYPE::MESSAGE;
        packet << msg;
        this->_send(this->_server, packet);
    }

    void send(const UDPPacket& packet)
    {
        this->_send(this->_server, packet);
    }

private:
     void _onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) override
    {
        std::cout << "Received packet from " << src.address().to_string() << ":" << src.port() << std::endl;
        const auto payload = packet.payload;

        std::cout << "Received: " << payload << " (seq: " << packet.sequence_number
                  << ", ack: " << packet.ack_number << ")" << std::endl;
    }

protected:
private:
    asio::ip::udp::endpoint _server;
    bool _running;
    sf::RenderWindow _window;
};
