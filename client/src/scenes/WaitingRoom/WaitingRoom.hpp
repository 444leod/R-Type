/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef WAITING_ROOM_HPP
#define WAITING_ROOM_HPP

#include "Registry.hpp"
#include "EventDispatcher.hpp"
#include "Components.hpp"
#include "NetworkedScene.hpp"
#include "UDPPacket.hpp"
#include <chrono>
#include <functional>

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

class WaitingRoom final : public NetworkAgent, public AScene {
public:
    WaitingRoom(ISceneManager& m, const std::string& n, asio::io_context& io_ctx) : NetworkAgent(io_ctx,  0), AScene(m, n)
    {
        std::cout << "WaitingRoom started, listening on port: " << this->_port << "..." << std::endl;
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
    void _onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) override;

    void _onConnect(UDPPacket& packet);
    void _onDisconnect(UDPPacket& packet);
    void _onMessage(UDPPacket& packet);
    void _onStart(UDPPacket& packet);

    void send(const UDPPacket& packet);

public:
private:
    asio::ip::udp::endpoint _server{};

    sf::Font font = get_default_font();

    bool _connected = false;

    Registry _registry;
    EventDispatcher _eventDispatcher;

    std::map<PACKET_TYPE, std::function<void(UDPPacket& packet)>> _packet_handlers = {
        {PACKET_TYPE::CONNECT,      [this](UDPPacket& packet)   { this->_onConnect(packet); }},
        {PACKET_TYPE::DISCONNECT,   [this](UDPPacket& packet)   { this->_onDisconnect(packet); }},
        {PACKET_TYPE::MESSAGE,      [this](UDPPacket& packet)   { this->_onMessage(packet); }},
        {PACKET_TYPE::START,        [this](UDPPacket& packet)   { this->_onStart(packet); }}
    };

//    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
//        {"exit",    [this](const std::vector<std::string>& args) { this->_onExit(args); }},
//        {"start",   [this](const std::vector<std::string>& args) { this->_onStart(args); }},
//        {"",        [this](const std::vector<std::string>& args) { }}
//    };

    // PlayerMovement _playerMovement{_registry};

};



#endif //GAME_HPP
