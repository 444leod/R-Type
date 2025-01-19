/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"
#include "../../Room/RoomManager.hpp"
#include <sstream>

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
    _executeUpdateSystems(deltaTime);

    auto& roomManager = RoomManager::instance();
    for (const auto& room : roomManager.getRooms()) {
        // if (room.isStarted && room.gameScene) {
        //     room.gameScene->update(deltaTime);
        //     roomManager.broadcastRoomState(room.id, getModule<ANetworkSceneModule>());
        // }
    }

    if (isInputAvailable()) {
        std::string command;
        std::getline(std::cin, command);
        handleCommand(command);
        std::cout << "> " << std::flush;
    }
}

void WaitingRoom::handleCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "rooms") {
        listRooms();
    } else if (cmd == "create") {
        std::string name;
        int maxPlayersTemp;
        if (iss >> name >> maxPlayersTemp) {
            if (maxPlayersTemp < 0 || maxPlayersTemp > 255) {
                std::cout << "Invalid number of players" << std::endl;
                return;
            }
            std::cout << "Creating room " << name << " with " << static_cast<int>(maxPlayersTemp) << " players" << std::endl;
            createRoom(name, static_cast<std::uint8_t>(maxPlayersTemp));
        } else {
            std::cout << "Usage: create <room_name> <max_players>" << std::endl;
        }
    } else if (cmd == "start") {
        int roomIdTemp;
        if (iss >> roomIdTemp) {
            if (roomIdTemp < 0) {
                std::cout << "Invalid room ID" << std::endl;
                return;
            }
            std::cout << "Starting room " << roomIdTemp << std::endl;
            startRoom(static_cast<std::uint32_t>(roomIdTemp));
        } else {
            std::cout << "Usage: start <room_id>" << std::endl;
        }
    } else if (cmd == "help") {
        showHelp();
    }
}

void WaitingRoom::listRooms() {
    auto rooms = RoomManager::instance().getRooms();
    if (rooms.empty()) {
        std::cout << "No rooms available" << std::endl;
        return;
    }

    std::cout << "Available rooms:" << std::endl;
    for (const auto& room : rooms) {
        std::cout << "Room " << room.id << " (" << room.name << "): "
                 << room.players.size() << "/" << static_cast<int>(room.maxPlayers)
                 << " players" << (room.isStarted ? " [STARTED]" : "[WAITING FOR START]") << std::endl;
    }
}

void WaitingRoom::startRoom(std::uint32_t roomId) {
    const auto net = this->getModule<ANetworkSceneModule>();
    if (!net) {
        std::cout << "Network module not found" << std::endl;
        return;
    }

    RoomManager::instance().startRoom(roomId, net);
}

void WaitingRoom::onEnter()
{
    _registry.clear();

    auto exitButtonEntity = _registry.create();
    // _registry.addComponent<Transform>(exitButtonEntity, Transform { .x = 20.0f, .y = 20.0f});
    // sf::RectangleShape exitButtonShape({100.0f, 40.0f});
    // exitButtonShape.setFillColor(sf::Color::Red);
    // _registry.addComponent<Button>(exitButtonEntity, {.shape exitButtonShape, "Exit", [this](){ game::RestrictedGame::instance().stop(); }});

    auto startButtonEntity = _registry.create();
    // _registry.addComponent<Transform>(startButtonEntity, Transform { .x = 140.0f, .y = 20.0f});
    // sf::RectangleShape startButtonShape({100.0f, 40.0f});
    // startButtonShape.setFillColor(sf::Color::Green);
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
    _registry.clear();
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

void WaitingRoom::createRoom(const std::string& name, std::uint8_t maxPlayers) {
    if (maxPlayers < 1) {
        std::cout << "Room must allow at least 1 players" << std::endl;
        return;
    }
    if (maxPlayers > 10) { // lets be crazy
        std::cout << "Room cannot have more than 10 players" << std::endl;
        return;
    }

    auto room = RoomManager::instance().createRoom(name, maxPlayers);
    std::cout << "Created room " << room->id << " (" << name << ") for " << static_cast<int>(maxPlayers) << " players" << std::endl;
}

void WaitingRoom::showHelp() {
    std::cout << "Available commands:" << std::endl
              << "  rooms                          - List all rooms" << std::endl
              << "  create <name> <max_players>    - Create a new room" << std::endl
              << "  start <room_id>               - Start a room" << std::endl
              << "  help                          - Show this help" << std::endl;
}
