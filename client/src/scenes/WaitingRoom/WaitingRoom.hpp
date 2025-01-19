/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef WAITING_ROOM_HPP
#define WAITING_ROOM_HPP

#include <chrono>
#include <optional>

#include <ECS/Registry.hpp>
#include <ECS/EventDispatcher.hpp>
#include <Engine/AScene.hpp>

#include "PremadeSystems/Update/CollisionSystem.hpp"

class WaitingRoom final : public engine::AScene {
public:
    explicit WaitingRoom(const std::string& name) : AScene(name)
    {
        _updateSystems.push_back(std::make_unique<CollisionSystem>());
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const engine::AScene& lastScene) override;

    void onExit() override;

    void onExit(const engine::AScene& nextScene) override;

private:
    std::optional<std::uint32_t> _id = std::nullopt;

    ecs::EventDispatcher _eventDispatcher;

    double _connectionTimer = 0;
    std::uint8_t _connectionAttempts = 0;
    double _pointTimer = 0;
    std::uint8_t _pointNumber = 0;

    void handleCommand(const std::string& command);
    void joinRoom(std::uint32_t roomId);
    void showHelp();
};



#endif //WAITING_ROOM_HPP
