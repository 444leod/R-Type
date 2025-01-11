/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"
#include "ecs/Registry.hpp"
#include <algorithm>
#include <cmath>

#include <iostream>
#include <SFML/Graphics.hpp>
#include "PacketTypes.hpp"
#include "NetworkModules/ANetworkSceneModule.hpp"
#include "Components.hpp"
#include "../../Systems/RemoveClientSystem.hpp"

inline bool isInputAvailable()
{
    fd_set readfds;
    struct timeval tv{};
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

void WaitingRoom::update(const double& deltaTime)
{
    if (isInputAvailable()) {
        if (std::string input;!std::getline(std::cin, input))
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

    this->_executeUpdateSystems(deltaTime);
}

void WaitingRoom::onEnter()
{
    _registry.clear();

    float y = 50.0f;
    // for (const auto& client : CLIENTS) { //todo: update this
    //     auto entity = _registry.create();
    //     _registry.addComponent<Position>(entity, {50.0f, y});
    //     sf::Text clientText("Client " + std::to_string(client.id), _font, 20);
    //     clientText.setFillColor(sf::Color::White);
    //     _registry.addComponent<Text>(entity, {clientText});
    //     y += 30.0f;
    // }

    auto exitButtonEntity = _registry.create();
    _registry.addComponent<Position>(exitButtonEntity, {20.0f, 20.0f});
    sf::RectangleShape exitButtonShape({100.0f, 40.0f});
    exitButtonShape.setFillColor(sf::Color::Red);
    _registry.addComponent<RectangleButton>(exitButtonEntity, {exitButtonShape, "Exit", [this](){ game::RestrictedGame::instance().stop(); }});

    auto startButtonEntity = _registry.create();
    _registry.addComponent<Position>(startButtonEntity, {140.0f, 20.0f});
    sf::RectangleShape startButtonShape({100.0f, 40.0f});
    startButtonShape.setFillColor(sf::Color::Green);
    _registry.addComponent<RectangleButton>(startButtonEntity, {startButtonShape, "Start", [this](){
       //TODO: start the game
    }});

    const auto net = this->getModule<ANetworkSceneModule>();
    if (net == nullptr)
    {
        std::cout << "No network module found, exiting..." << std::endl;
        game::RestrictedGame::instance().stop();
        return;
    }
}

void WaitingRoom::onEnter(const AScene& lastScene)
{
}

void WaitingRoom::onExit()
{
    ntw::UDPPacket packet;
    packet << PACKET_TYPE::DISCONNECT;

    const auto net = this->getModule<ANetworkSceneModule>();
    if (net == nullptr)
        return;

    net->sendPacket(packet);
}

void WaitingRoom::onExit(const AScene& nextScene)
{
    std::cout << "Exiting to " << nextScene.name() << std::endl;
}

void WaitingRoom::_startGame(const std::vector<std::string> &)
{
    std::cout << "Starting the game..." << std::endl;
    game::RestrictedGame::instance().scenes().load("game");
}
