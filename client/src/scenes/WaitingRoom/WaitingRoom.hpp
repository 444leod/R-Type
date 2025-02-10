/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef WAITING_ROOM_HPP
#define WAITING_ROOM_HPP

#include <optional>

#include <ECS/EventDispatcher.hpp>
#include <ECS/Registry.hpp>
#include <Engine/AScene.hpp>

#include "PremadeSystems/Update/CollisionSystem.hpp"
#include "PremadeSystems/Event/InputActivateSystem.hpp"
#include "PremadeSystems/Event/InputTypeSystem.hpp"

class WaitingRoom final : public engine::AScene
{
  public:
    explicit WaitingRoom(const std::string& name) : AScene(name) { _updateSystems.push_back(std::make_unique<CollisionSystem>()); }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const engine::AScene& lastScene) override;

    void onExit() override;

    void onExit(const engine::AScene& nextScene) override;

  private:
    InputActivateSystem _inputActivateSystem;
    InputTypeSystem _inputTypeSystem;
  
    std::optional<std::uint32_t> _id = std::nullopt;

    ecs::EventDispatcher _eventDispatcher;

    double _connectionTimer = 0;
    std::uint8_t _connectionAttempts = 0;
    double _pointTimer = 0;
    std::uint8_t _pointNumber = 0;
    std::uint32_t _statusTextId = 0;
    bool _connecting = false;
    bool _connected = false;
};

#endif // WAITING_ROOM_HPP
