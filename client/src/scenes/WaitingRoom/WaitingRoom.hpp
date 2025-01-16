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

#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"
#include "engine/AScene.hpp"
#include "BaseComponents.hpp"

#include <BaseSystems/Update/CollisionSystem.hpp>

class WaitingRoom final : public AScene {
public:
    WaitingRoom(RestrictedSceneManager& m, ecs::Registry& r, const std::string& n) : AScene(m, r, n)
    {
        _updateSystems.push_back(std::make_unique<CollisionSystem>(_registry));
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

private:
    std::optional<std::uint32_t> _id = std::nullopt;

    ecs::EventDispatcher _eventDispatcher;

    double _connectionTimer = 0;
    std::uint8_t _connectionAttempts = 0;
    double _pointTimer = 0;
    std::uint8_t _pointNumber = 0;
};



#endif //WAITING_ROOM_HPP
