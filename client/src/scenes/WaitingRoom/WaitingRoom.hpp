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
#include "AScene.hpp"
#include <chrono>
#include <functional>
#include <optional>
#include "Global.hpp"

inline sf::Font get_default_font() {
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");
    return font;
}

class WaitingRoom final : public AScene {
public:
    WaitingRoom(ISceneManager& m, Registry& r, const std::string& n) : AScene(m, r, n)
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

private:
/*     void _onConnect(UDPPacket& packet);
    void _onDisconnect(UDPPacket& packet);
    void _onMessage(UDPPacket& packet);
    void _onStart(UDPPacket& packet);

    void send(const UDPPacket& packet); */

public:
private:
    sf::Font _font = get_default_font();

    std::optional<std::uint32_t> _id = std::nullopt;

    Registry _registry;
    EventDispatcher _eventDispatcher;

//    std::map<std::string, std::function<void(const std::vector<std::string>&)>> _command_handlers = {
//        {"exit",    [this](const std::vector<std::string>& args) { this->_onExit(args); }},
//        {"start",   [this](const std::vector<std::string>& args) { this->_onStart(args); }},
//        {"",        [this](const std::vector<std::string>& args) { }}
//    };

    // PlayerMovement _playerMovement{_registry};

};



#endif //GAME_HPP
