/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"

#include <ECS/Registry.hpp>

#include "PremadeModules/Network/ANetworkSceneModule.hpp"

#include "PremadeComponents/Transform.hpp"

#include "PacketTypes.hpp"

#include <iostream>
#include <cmath>

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

    auto exitButtonEntity = _registry.create();
    _registry.addComponent<Transform>(exitButtonEntity, Transform { .x = 20.0f, .y = 20.0f});
    sf::RectangleShape exitButtonShape({100.0f, 40.0f});
    exitButtonShape.setFillColor(sf::Color::Red);
    // _registry.addComponent<Button>(exitButtonEntity, {.shape exitButtonShape, "Exit", [this](){ game::RestrictedGame::instance().stop(); }});

    auto startButtonEntity = _registry.create();
    _registry.addComponent<Transform>(startButtonEntity, Transform { .x = 140.0f, .y = 20.0f});
    sf::RectangleShape startButtonShape({100.0f, 40.0f});
    startButtonShape.setFillColor(sf::Color::Green);
    // _registry.addComponent<Button>(startButtonEntity, {startButtonShape, "Start", [this](){
    //    //TODO: start the game
    // }});

    if (this->getModule<ANetworkSceneModule>() == nullptr)
    {
        std::cout << "No Network module found, exiting..." << std::endl;
        throw std::runtime_error("No Network module found");
    }
}

void WaitingRoom::onEnter(const AScene& lastScene)
{
    this->onEnter();
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
    const auto net = this->getModule<ANetworkSceneModule>();

    if (net->clients().size() < 1)
    {
        std::cout << "Not enough players to start the game" << std::endl;
        return;
    }

    std::cout << "Starting the game..." << std::endl;
    engine::RestrictedGame::instance().scenes().load("game");

    ntw::UDPPacket packet;
    packet << PACKET_TYPE::START;

    net->sendPacket(packet);
}
