/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Client
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include "NetworkAgent.hpp"

#pragma once

class Client: public NetworkAgent
{
public:
    Client(asio::io_context& ctx): NetworkAgent(ctx) {}
    ~Client() override = default;

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
    [[nodiscard]] bool running() const noexcept { return this->_window.isOpen() && this->_running; }

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

        PACKET_TYPE packet_type{};
        packet >> packet_type;
        if (_packet_handlers.contains(packet_type))
            _packet_handlers.at(packet_type)(packet);

    }

protected:
private:
    asio::ip::udp::endpoint _server;
    bool _running = true;
    sf::RenderWindow _window;
    std::map<PACKET_TYPE, std::function<void(UDPPacket&)>> _packet_handlers = {
        {
            PACKET_TYPE::CONNECT, [&](UDPPacket& packet)
            {
                std::cout << "Received CONNECT packet." << std::endl;
                std::uint32_t client_id;
                packet >> client_id;
                this->_client_id = client_id;
                std::cout << "Client ID: " << client_id << std::endl;
            }
        },
        {
            PACKET_TYPE::DISCONNECT, [&](UDPPacket& packet)
            {
                std::cout << "Received DISCONNECT packet." << std::endl;
                this->_running = false;
            }
        },
        {
            PACKET_TYPE::MESSAGE, [&](UDPPacket& packet)
            {
                std::string msg;
                packet >> msg;
                std::cout << "Received message: " << msg << std::endl;
            }
        },
        {
            PACKET_TYPE::START, [&](UDPPacket& packet)
            {
                std::cout << "Received START packet." << std::endl;
            }
        }
    };

    std::optional<std::uint32_t> _client_id;
};
