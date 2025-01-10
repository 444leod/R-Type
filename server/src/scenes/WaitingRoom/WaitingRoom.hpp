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

#include "Systems/DrawMenuButtonsSystem.hpp"
#include "PacketTypes.hpp"

#include <chrono>
#include <functional>
#include <memory>

class WaitingRoom final : public AScene {
public:
    WaitingRoom(RestrictedSceneManager& m, ecs::Registry& r, const std::string& n) : AScene(m, r, n)
    {
        this->_renderSystems.push_back(std::make_unique<DrawTextsSystem>(_registry));
        this->_renderSystems.push_back(std::make_unique<DrawMenuButtonsSystem>(_registry));
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

private:

public:
private:
    ecs::Registry _registry;
    ecs::EventDispatcher _eventDispatcher;

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
        {"exit",    [this](const std::vector<std::string>& args) { this->_onExit(args); }},
        {"start",   [this](const std::vector<std::string>& args) { this->_onStart(args); }},
        {"",        [this](const std::vector<std::string>& args) { }}
    };

    // PlayerMovement _playerMovement{_registry};
};



#endif //GAME_HPP
