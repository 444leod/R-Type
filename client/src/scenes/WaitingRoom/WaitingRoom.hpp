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
/*     void _onConnect(ntw::UDPPacket& packet);
    void _onDisconnect(ntw::UDPPacket& packet);
    void _onMessage(ntw::UDPPacket& packet);
    void _onStart(ntw::UDPPacket& packet);

    void send(const UDPPacket& packet); */

public:
private:
    std::optional<std::uint32_t> _id = std::nullopt;

    ecs::EventDispatcher _eventDispatcher;

//    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
//        {"exit",    [this](const std::vector<std::string>& args) { this->_onExit(args); }},
//        {"start",   [this](const std::vector<std::string>& args) { this->_onStart(args); }},
//        {"",        [this](const std::vector<std::string>& args) { }}
//    };

    // PlayerMovement _playerMovement{_registry};

};



#endif //WAITING_ROOM_HPP
