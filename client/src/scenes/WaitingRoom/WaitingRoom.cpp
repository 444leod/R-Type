/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"
#include "Registry.hpp"
#include <algorithm>
#include <cmath>
#include <config.h>
#include <iostream>
#include <ranges>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

void WaitingRoom::initialize()
{
}

void WaitingRoom::update(const double deltaTime, const sf::RenderWindow &window)
{
}

void WaitingRoom::render(sf::RenderWindow& window)
{
    if (this->_id.has_value()) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setString("Connected to the server");
        text.setPosition(10, 10);
        window.draw(text);
    } else {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setString("Connecting to the server...");
        text.setPosition(10, 10);
        window.draw(text);
    }
}

void WaitingRoom::onEvent(sf::Event &event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Space:
                    break;
                case sf::Keyboard::B: {
                     break;
                }
                default:
                    // _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = true});
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code) {
                default:
                    // _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = false});
                    break;
            }
            break;
        case sf::Event::MouseButtonPressed:
            break;
        default:break;
    }
}

void WaitingRoom::onEnter() {
    _registry.clear();

    std::cout << "Game is running..." << std::endl;

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
    SERVER = asio::ip::udp::endpoint(addr, port);

    UDPPacket packet;
    packet << PACKET_TYPE::CONNECT;
    packet << "ClientName";

    _manager.send(SERVER, packet);
}

void WaitingRoom::onEnter(const AScene& lastScene)
{
}

void WaitingRoom::onExit()
{
}

void WaitingRoom::onExit(const AScene& nextScene)
{
}

void WaitingRoom::onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet)
{
    const auto payload = packet.payload;

    // std::cout << "Received: " << payload << " (seq: " << packet.sequence_number
    //           << ", ack: " << packet.ack_number << ")" << std::endl;

    PACKET_TYPE packet_type{};
    packet >> packet_type;
    if (_packet_handlers.contains(packet_type))
        _packet_handlers.at(packet_type)(packet);
}

void WaitingRoom::send(const UDPPacket& packet)
{
    _manager.send(SERVER, packet);
}

void WaitingRoom::_onConnect(UDPPacket& packet)
{

    std::uint32_t id;
    packet >> id;

    this->_id = id;
    std::cout << "Connected to the server" << std::endl;
}

void WaitingRoom::_onDisconnect(UDPPacket& packet)
{
    std::cout << "Disconnection from the server" << std::endl;
    _manager.stop();
}

void WaitingRoom::_onMessage(UDPPacket& packet)
{
    std::string message;
    packet >> message;
    std::cout << "Message from " << SERVER << ": " << message << std::endl;
}

void WaitingRoom::_onStart(UDPPacket& packet)
{
    std::cout << "Game starting..." << std::endl;
    _manager.load("Level1");
}