/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"
#include <ECS/Registry.hpp>


#include "PremadeModules/Network/ANetworkSceneModule.hpp"
#include "PremadeModules/Rendering/ASceneRenderingModule.hpp"

#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Displayable/Text.hpp"
#include "PremadeComponents/Displayable/Button.hpp"

#include "SharedComponents/Client.hpp"

#include <iostream>
#include <sstream>
#include <sys/select.h>

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
}

void WaitingRoom::update(const double& deltaTime)
{
    if (_registry.view<Client>().size() != 0)
    {
        if (isInputAvailable()) {
            std::string command;
            std::getline(std::cin, command);
            handleCommand(command);
            std::cout << "> " << std::flush;
        }
    }
    else
    {
        if (_connectionAttempts > 5)
        {
            std::cout << "\nFailed to connect to the server" << std::endl;
            engine::RestrictedGame::instance().stop();
            return;
        }

        _connectionTimer += deltaTime;
        _pointTimer += deltaTime;

        if (_connectionTimer > 3) {
            _connectionTimer = 0;
            _connectionAttempts++;
            const auto net = this->getModule<ANetworkSceneModule>();
            net->resend();
        }

        if (_pointTimer > 1) {
            _pointTimer = 0;
            _pointNumber = (_pointNumber + 1) % 4;
            std::cout << "\rConnecting to the server" << std::string(_pointNumber, '.') << std::string(3 - _pointNumber, ' ') << std::flush;
        }
    }
    this->_executeUpdateSystems(deltaTime);
}

void WaitingRoom::onEnter() {
    const auto enttext = _registry.create();
    _registry.addComponent<Text>(enttext, Text{
        .font = "./assets/arial.ttf",
        .message = "Connecting to server...",
        .fontSize = 30u,
        .color = Color(255, 255, 255)
    });
    _registry.addComponent(enttext, Transform { .x = 10, .y = 10 });

    const auto button = _registry.create();
    _registry.addComponent<Button>(button, Button {
        .shape = shape::Rectangle(200, 50, Color(100, 100, 100), Color(0, 0, 0), 2),
        .onClick = [] () { std::cout << "Button clicked" << std::endl; },
        .label = Text{
            .font = "./assets/arial.ttf",
            .message = "Click me",
           .fontSize = 30u,
           .color = Color(255, 255, 255)
        }
    });
    _registry.addComponent(button, Transform { .x = 100, .y = 100, .rotation = 20 });
}

void WaitingRoom::onEnter(const AScene& lastScene)
{
}

void WaitingRoom::onExit()
{
}

void WaitingRoom::onExit(const AScene& nextScene) {}

void WaitingRoom::handleCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "join") {
        int roomIdTemp;
        if (iss >> roomIdTemp) {
            if (roomIdTemp < 0) {
                std::cout << "Invalid room ID" << std::endl;
                return;
            }
            std::cout << "Joining room " << roomIdTemp << std::endl;
            joinRoom(static_cast<std::uint32_t>(roomIdTemp));
        } else {
            std::cout << "Usage: join <room_id>" << std::endl;
        }
    } else if (cmd == "help") {
        showHelp();
    }
}

void WaitingRoom::joinRoom(std::uint32_t roomId) {
    const auto net = this->getModule<ANetworkSceneModule>();
    if (!net) {
        std::cout << "Network module not found" << std::endl;
        return;
    }

    ntw::UDPPacket packet;
    packet << PACKET_TYPE::JOIN_ROOM << roomId;
    net->sendPacket(packet);
}

void WaitingRoom::showHelp() {
    std::cout << "Available commands:" << std::endl
              << "  join <room_id>    - Join a room" << std::endl
              << "  help              - Show this help" << std::endl;
}
