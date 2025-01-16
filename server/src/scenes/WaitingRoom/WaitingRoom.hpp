/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef WAITING_ROOM_HPP
#define WAITING_ROOM_HPP

#include "engine/AScene.hpp"
#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"
#include "network/NetworkAgent.hpp"
#include "BaseComponents.hpp"
#include "BaseSystems/Render/DrawTextsSystem.hpp"

#include "PacketTypes.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include "BaseSystems/Update/CollisionSystem.hpp"

#include <engine/RestrictedGame.hpp>

class WaitingRoom final : public AScene {
public:
    WaitingRoom(RestrictedSceneManager& m, ecs::Registry& r, const std::string& n) : AScene(m, r, n)
    {
        this->_updateSystems.push_back(std::make_unique<CollisionSystem>(_registry));
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

private:
    void _startGame(const std::vector<std::string>& args);

public:
private:
    ecs::Registry _registry;

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
        {"exit",    [this](const std::vector<std::string>& args) { game::RestrictedGame::instance().stop(); }},
        {"start",   [this](const std::vector<std::string>& args) { this->_startGame(args); }},
        {"",        [this](const std::vector<std::string>& args) { }}
    };
};



#endif //GAME_HPP
