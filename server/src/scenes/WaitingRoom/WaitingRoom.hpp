/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef WAITING_ROOM_HPP
#define WAITING_ROOM_HPP

#include "scenes/AScene.hpp"
#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"
#include "network/NetworkAgent.hpp"
#include "Components.hpp"
#include <chrono>
#include <functional>

class WaitingRoom final : public AScene {
public:
    WaitingRoom(ISceneManager& m, ecs::Registry& r, const std::string& n) : AScene(m, r, n)
    {
        this->_font.loadFromFile("assets/arial.ttf");
    }

    void initialize() override;

    void update(double deltaTime, const sf::RenderWindow &window) override;

    void render(sf::RenderWindow &window) override;

    void onEvent(sf::Event &event) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

private:

    void _onConnect(const ntw::ClientInformations& src, ntw::UDPPacket& packet);
    void _onDisconnect(const ntw::ClientInformations& src, ntw::UDPPacket& packet);
    void _onMessage(const ntw::ClientInformations& src, ntw::UDPPacket& packet);

    void _onExit(const std::vector<std::string>& args);
    void _onStart(const std::vector<std::string>& args);

    //void _broadcast(const UDPPacket& packet);


public:
private:
    ecs::Registry _registry;
    ecs::EventDispatcher _eventDispatcher;

    sf::Font _font;

    /* std::map<PACKET_TYPE, std::function<void(const ClientInformations& client, UDPPacket& packet)>> _packet_handlers = {
        {PACKET_TYPE::CONNECT,      [this](const ClientInformations& src, UDPPacket& packet)   { this->_onConnect(src, packet); }},
        {PACKET_TYPE::DISCONNECT,   [this](const ClientInformations& src, UDPPacket& packet)   { this->_onDisconnect(src, packet); }},
        {PACKET_TYPE::MESSAGE,      [this](const ClientInformations& src, UDPPacket& packet)   { this->_onMessage(src, packet); }}
    }; */

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
        {"exit",    [this](const std::vector<std::string>& args) { this->_onExit(args); }},
        {"start",   [this](const std::vector<std::string>& args) { this->_onStart(args); }},
        {"",        [this](const std::vector<std::string>& args) { }}
    };

    // PlayerMovement _playerMovement{_registry};

};



#endif //GAME_HPP
