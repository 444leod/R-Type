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
    this->_server = asio::ip::udp::endpoint(addr, port);

    std::cout << "Connecting to the server: " << ip << ":" << port << std::endl;

    UDPPacket packet;
    packet << PACKET_TYPE::CONNECT;
    packet << "CONNECT";

    this->_send(this->_server, packet);
    std::cout << "Connecting to the server..." << std::endl;
}

void WaitingRoom::update(const double deltaTime, const sf::RenderWindow &window)
{
}

void WaitingRoom::render(sf::RenderWindow& window)
{
    if (this->_connected) {
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

}

void WaitingRoom::onEnter(const AScene& lastScene)
{
}

void WaitingRoom::onExit() {
    std::cout << "Exiting totally..." << std::endl;
}

void WaitingRoom::onExit(const AScene& nextScene) {
}

void WaitingRoom::_onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) {

    std::cout << "Received packet from " << src.address().to_string() << ":" << src.port() << std::endl;
    const auto payload = packet.payload;

    std::cout << "Received: " << payload << " (seq: " << packet.sequence_number
              << ", ack: " << packet.ack_number << ")" << std::endl;

    PACKET_TYPE packet_type{};
    packet >> packet_type;
    if (_packet_handlers.contains(packet_type))
        _packet_handlers.at(packet_type)(packet);
}

void WaitingRoom::send(const UDPPacket& packet) {
    this->_send(this->_server, packet);
}

void WaitingRoom::_onConnect(UDPPacket& packet) {
//    std::string name;
//    packet >> name;
//    std::cout << "Client connected: " << name << std::endl;
//
//    UDPPacket response;
//    response << PACKET_TYPE::CONNECT;
//    response << source.id.value();
    std::cout << "Connected to the server" << std::endl;
    this->_connected = true;
}

void WaitingRoom::_onDisconnect(UDPPacket& packet) {
//    std::cout << "Client disconnected: " << source.endpoint << std::endl;
//    this->_clients.erase(std::remove_if(this->_clients.begin(), this->_clients.end(), [&source](const auto& client) {
//        return client.endpoint == source.endpoint;
//    }), this->_clients.end());

    //send informations about players connected
}

void WaitingRoom::_onMessage(UDPPacket& packet) {
    std::string message;
    packet >> message;
    std::cout << "Message from " << this->_server << ": " << message << std::endl;
}

void WaitingRoom::_onStart(UDPPacket& packet) {
    std::cout << "Game starting..." << std::endl;
}