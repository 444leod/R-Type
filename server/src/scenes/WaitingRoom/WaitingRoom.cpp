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
#include "Global.hpp"

inline bool isInputAvailable()
{
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    return select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv) > 0;
}

void WaitingRoom::initialize()
{
    std::cout << "Game is running..." << std::endl;
    std::cout << "> " << std::flush;
}

void WaitingRoom::update(const double deltaTime, const sf::RenderWindow &window)
{
    if (isInputAvailable())
    {
        std::string input;
        if (!std::getline(std::cin, input))
        {
            if (std::cin.eof())
            {
                _command_handlers["exit"]({});
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        else
        {
            if (_command_handlers.contains(input))
            {
                std::vector<std::string> args;
                std::istringstream iss(input);
                std::string arg;
                while (iss >> arg)
                    args.push_back(arg);
                _command_handlers.at(input)(args);
            }
        }

        std::cout << "> " << std::flush;
    }
}

void WaitingRoom::render(sf::RenderWindow &window)
{
    _executeRenderSystems(window);
}

void WaitingRoom::onEvent(sf::Event &event)
{
    //! Will not refactor this part, not supposed to have a system here
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        _registry.view<Position, RectangleButton>().each([&](const auto &entity, auto &pos, auto &button)
                                                         {
            if (button.shape.getGlobalBounds().contains(mousePos)) {
                button.onClick();
                return;
            } });
    }

    switch (event.type)
    {
    case sf::Event::KeyPressed:
        switch (event.key.code)
        {
        case sf::Keyboard::Space:
            break;
        case sf::Keyboard::B:
        {
            break;
        }
        default:
            // _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = true});
            break;
        }
        break;
    case sf::Event::KeyReleased:
        switch (event.key.code)
        {
        default:
            // _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = false});
            break;
        }
        break;
    case sf::Event::MouseButtonPressed:
        break;
    default:
        break;
    }
}

void WaitingRoom::onEnter()
{
    _registry.clear();

    float y = 50.0f;
    for (const auto &client : CLIENTS)
    {
        auto entity = _registry.create();
        Text clientText = {
            .message = "Client " + std::to_string(client.id),
            .fontSize = 20,
            .color = {255, 255, 255}};
        _registry.addComponent<Text>(entity, clientText);
        _registry.addComponent<Position>(entity, {50.0f, y});
        y += 30.0f;
    }

    auto exitButtonEntity = _registry.create();
    _registry.addComponent<Position>(exitButtonEntity, {20.0f, 20.0f});
    sf::RectangleShape exitButtonShape({100.0f, 40.0f});
    exitButtonShape.setFillColor(sf::Color::Red);
    _registry.addComponent<RectangleButton>(exitButtonEntity, {exitButtonShape, "Exit", [this]()
                                                               { _onExit({}); }});

    auto startButtonEntity = _registry.create();
    _registry.addComponent<Position>(startButtonEntity, {140.0f, 20.0f});
    sf::RectangleShape startButtonShape({100.0f, 40.0f});
    startButtonShape.setFillColor(sf::Color::Green);
    _registry.addComponent<RectangleButton>(startButtonEntity, {startButtonShape, "Start", [this]()
                                                                { _onStart({}); }});
}

void WaitingRoom::onEnter(const AScene &lastScene)
{
}

void WaitingRoom::onExit()
{
    UDPPacket packet;
    packet << PACKET_TYPE::DISCONNECT;

    this->_broadcast(packet);
}

void WaitingRoom::onExit(const AScene &nextScene)
{
}

void WaitingRoom::onPacketReceived(const asio::ip::udp::endpoint &src, UDPPacket &packet)
{

    const auto payload = packet.payload;
    // std::cout << "Received: " << payload << " (seq: " << packet.sequence_number
    //     << ", ack: " << packet.ack_number << ")" << std::endl;

    PACKET_TYPE packet_type{};
    packet >> packet_type;

    auto it = std::find_if(CLIENTS.begin(), CLIENTS.end(), [&src](const auto &client)
                           { return client.endpoint == src; });

    if (it == CLIENTS.end())
    {
        if (packet_type != PACKET_TYPE::CONNECT)
            return;
        CLIENTS.push_back(ClientInformations(src,
                                             (CLIENTS.size() > 4) ? ClientInformations::Type::VIEWER : ClientInformations::Type::PLAYER,
                                             CLIENTS.size()));

        std::cout << "New client, new size: " << CLIENTS.size() << std::endl;

        std::cout << "Client connected: " << src << std::endl;
        it = CLIENTS.end() - 1;
    }

    if (_packet_handlers.contains(packet_type))
        _packet_handlers.at(packet_type)(*it, packet);
}

void WaitingRoom::_broadcast(const UDPPacket &packet)
{
    for (auto &[endpoint, type, name, id] : CLIENTS)
    {
        _manager.send(endpoint, packet);
    }
}

void WaitingRoom::_onConnect(const ClientInformations &src, UDPPacket &packet)
{
    std::string name;
    packet >> name;
    std::cout << "Client connected: " << name << std::endl;

    UDPPacket response;
    response << PACKET_TYPE::CONNECT;
    response << src.id;

    _manager.send(src.endpoint, response);

    auto entity = _registry.create();
    Text clientText = {
        .message = "Client " + std::to_string(src.id),
        .fontSize = 20,
        .color = {255, 255, 255}
    };
    _registry.addComponent<Text>(entity, {clientText});
    _registry.addComponent<Position>(entity, {50.0f, 50.0f + 30.0f * CLIENTS.size()});
}

void WaitingRoom::_onDisconnect(const ClientInformations &src, UDPPacket &packet)
{
    std::cout << "Client disconnected: " << src.endpoint << std::endl;
    std::erase_if(CLIENTS, [&src](const auto &client)
                  { return client.endpoint == src.endpoint; });

    // send informations about players connected
}

void WaitingRoom::_onMessage(const ClientInformations &source, UDPPacket &packet)
{
    std::string message;
    packet >> message;
    std::cout << "Message from " << source.endpoint << ": " << message << std::endl;
}

void WaitingRoom::_onExit(const std::vector<std::string> &args)
{
    std::cout << "Exiting..." << std::endl;
    _manager.stop();

    this->_broadcast(UDPPacket{} << PACKET_TYPE::DISCONNECT);
}

void WaitingRoom::_onStart(const std::vector<std::string> &args)
{
    std::cout << "Starting game..." << std::endl;

    if (CLIENTS.empty())
    {
        std::cout << "Not enough players to start the game." << std::endl;
        return;
    }

    this->_broadcast(UDPPacket{} << PACKET_TYPE::START);
    _manager.load("Level1");
}
