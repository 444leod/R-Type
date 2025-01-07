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

    void _onConnect(const ntw::ClientInformation& src, ntw::UDPPacket& packet);
    void _onDisconnect(const ntw::ClientInformation& src, ntw::UDPPacket& packet);
    void _onMessage(const ntw::ClientInformation& src, ntw::UDPPacket& packet);

    void _onExit(const std::vector<std::string>& args);
    void _onStart(const std::vector<std::string>& args);

    //void _broadcast(const UDPPacket& packet);


public:
private:
    ecs::Registry _registry;
    ecs::EventDispatcher _eventDispatcher;

    sf::Font _font;

    /* std::map<PACKET_TYPE, std::function<void(const ClientInformation& client, ntw::UDPPacket& packet)>> _packet_handlers = {
        {PACKET_TYPE::CONNECT,      [this](const ClientInformation& src, ntw::UDPPacket& packet)   { this->_onConnect(src, packet); }},
        {PACKET_TYPE::DISCONNECT,   [this](const ClientInformation& src, ntw::UDPPacket& packet)   { this->_onDisconnect(src, packet); }},
        {PACKET_TYPE::MESSAGE,      [this](const ClientInformation& src, ntw::UDPPacket& packet)   { this->_onMessage(src, packet); }}
    }; */

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
        {"exit",    [this](const std::vector<std::string>& args) { this->_onExit(args); }},
        {"start",   [this](const std::vector<std::string>& args) { this->_onStart(args); }},
        {"",        [this](const std::vector<std::string>& args) { }}
    };

    // PlayerMovement _playerMovement{_registry};
};



#endif //GAME_HPP
