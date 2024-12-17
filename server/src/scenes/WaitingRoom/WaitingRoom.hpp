/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef WAITING_ROOM_HPP
#define WAITING_ROOM_HPP

#include "AScene.hpp"
#include "Registry.hpp"
#include "EventDispatcher.hpp"
#include "Components.hpp"
#include "NetworkedScene.hpp"
#include <chrono>

inline sf::Font get_default_font() {
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");
    return font;
}

struct ClientInformations {
    asio::ip::udp::endpoint endpoint;
    enum class Type {
        VIEWER,
        PLAYER
    } type;
    std::optional<std::string> name = std::nullopt;
    std::optional<std::uint32_t> id = std::nullopt;

};

class WaitingRoom final : public AScene {
public:
    WaitingRoom(ISceneManager& m, const std::string& n) : AScene(m, n)
    {
    }

    void initialize() override;

    void update(double deltaTime, const sf::RenderWindow &window) override;

    void render(sf::RenderWindow &window) override;

    void onEvent(sf::Event &event) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

    void onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) override;

private:

    void _onConnect(const ClientInformations& src, UDPPacket& packet);
    void _onDisconnect(const ClientInformations& src, UDPPacket& packet);
    void _onMessage(const ClientInformations& src, UDPPacket& packet);

    void _onExit(const std::vector<std::string>& args);
    void _onStart(const std::vector<std::string>& args);

    void _broadcast(const UDPPacket& packet);

public:
private:
    Registry _registry;
    EventDispatcher _eventDispatcher;

    sf::Font font = get_default_font();

    std::vector<ClientInformations> _clients;

    std::map<PACKET_TYPE, std::function<void(const ClientInformations& client, UDPPacket& packet)>> _packet_handlers = {
        {PACKET_TYPE::CONNECT,      [this](const ClientInformations& src, UDPPacket& packet)   { this->_onConnect(src, packet); }},
        {PACKET_TYPE::DISCONNECT,   [this](const ClientInformations& src, UDPPacket& packet)   { this->_onDisconnect(src, packet); }},
        {PACKET_TYPE::MESSAGE,      [this](const ClientInformations& src, UDPPacket& packet)   { this->_onMessage(src, packet); }}
    };

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
        {"exit",    [this](const std::vector<std::string>& args) { this->_onExit(args); }},
        {"start",   [this](const std::vector<std::string>& args) { this->_onStart(args); }},
        {"",        [this](const std::vector<std::string>& args) { }}
    };

    // PlayerMovement _playerMovement{_registry};

};



#endif //GAME_HPP
