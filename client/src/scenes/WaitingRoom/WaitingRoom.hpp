/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef WAITING_ROOM_HPP
#define WAITING_ROOM_HPP

#include "BaseComponents.hpp"
#include "EventDispatcher.hpp"
#include "Global.hpp"
#include "NetworkedScene.hpp"
#include "Registry.hpp"
#include "UDPPacket.hpp"
#include <chrono>
#include <functional>
#include <optional>

inline sf::Font get_default_font() {
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");
    return font;
}

class WaitingRoom final : public AScene {
  public:
    WaitingRoom(ISceneManager& m, const std::string& n) : AScene(m, n) {}

    void initialize() override;

    void update(double deltaTime, const sf::RenderWindow& window) override;

    void render(sf::RenderWindow& window) override;

    void onEvent(sf::Event& event) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

    void onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) override;

  private:
    void _onConnect(UDPPacket& packet);
    void _onDisconnect(UDPPacket& packet);
    void _onMessage(UDPPacket& packet);
    void _onStart(UDPPacket& packet);

    void send(const UDPPacket& packet);

  public:
  private:
    sf::Font font = get_default_font();

    std::optional<std::uint32_t> _id = std::nullopt;

    Registry _registry;
    EventDispatcher _eventDispatcher;

    std::map<PACKET_TYPE, std::function<void(UDPPacket& packet)>> _packet_handlers = {{PACKET_TYPE::CONNECT, [this](UDPPacket& packet) { this->_onConnect(packet); }}, {PACKET_TYPE::DISCONNECT, [this](UDPPacket& packet) { this->_onDisconnect(packet); }},
        {PACKET_TYPE::MESSAGE, [this](UDPPacket& packet) { this->_onMessage(packet); }}, {PACKET_TYPE::START, [this](UDPPacket& packet) { this->_onStart(packet); }}};

    //    std::map<std::string, std::function<void(const
    //    std::vector<std::string>&)>> _command_handlers = {
    //        {"exit",    [this](const std::vector<std::string>& args) {
    //        this->_onExit(args); }},
    //        {"start",   [this](const std::vector<std::string>& args) {
    //        this->_onStart(args); }},
    //        {"",        [this](const std::vector<std::string>& args) { }}
    //    };

    // PlayerMovement _playerMovement{_registry};
};

#endif // GAME_HPP
