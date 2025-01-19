/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef WAITING_ROOM_HPP
#define WAITING_ROOM_HPP

#include <ECS/Registry.hpp>

#include <Engine/AScene.hpp>
#include <Engine/RestrictedGame.hpp>

#include "PremadeSystems/Update/CollisionSystem.hpp"

#include <functional>
#include <memory>

class WaitingRoom final : public engine::AScene {
public:
    explicit WaitingRoom(const std::string& name) : AScene(name)
    {
        this->_updateSystems.push_back(std::make_unique<CollisionSystem>());
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

private:
    void _startGame(const std::vector<std::string>& args);
    void handleCommand(const std::string& command);
    void listRooms();
    void startRoom(std::uint32_t roomId);
    void createRoom(const std::string& name, std::uint8_t maxPlayers);
    void showHelp();

public:
private:
    ecs::Registry _registry;

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
        {"exit",    [this](const std::vector<std::string>& args) { engine::RestrictedGame::instance().stop(); }},
        {"start",   [this](const std::vector<std::string>& args) { this->_startGame(args); }},
        {"",        [this](const std::vector<std::string>& args) { }}
    };
};



#endif //GAME_HPP
