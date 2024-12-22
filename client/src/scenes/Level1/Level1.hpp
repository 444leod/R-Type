/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef LEVEL_1_HPP
#define LEVEL_1_HPP

#include "AScene.hpp"
#include "Registry.hpp"
#include "EventDispatcher.hpp"
#include "Components.hpp"
#include "Events/InputHandler.hpp"
#include "Events/PacketHandler.hpp"
#include "BaseSystems/Update/ParalaxSystem.hpp"
#include "BaseSystems/Render/DrawSpritesSystem.hpp"
#include <chrono>

class Level1 final : public AScene {
public:
    Level1(ISceneManager& m, const std::string& n) : AScene(m, n) {
        _spaceshipTex.loadFromFile("assets/r-typesheet42.gif", sf::IntRect(0, 0, 34, 18));
        _projectileTex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(0, 91, 48, 16));
        _backgroundTex.loadFromFile("assets/rtype-background.png", sf::IntRect(0, 243, 3072, 205));
        _bugTex.loadFromFile("assets/r-typesheet8.gif");
        _explosionTex.loadFromFile("assets/r-typesheet44.gif", sf::IntRect(131, 0, 192, 32));
        _eventDispatcher.link<UserInput>(&_inputHandler);
        _eventDispatcher.link<PacketInformations>(&_packetHandler);
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
    void addProjectile(const Transform& transform);
    void addBug(const Transform& transform);

public:
private:
    EventDispatcher _eventDispatcher;

    asio::ip::udp::endpoint _server{};

    InputHandler _inputHandler{_registry, _manager};
    PacketHandler _packetHandler{_registry, _manager};

    std::vector<ClientInformations> _clients;

    //this is temporary, if this texture get deleted, then the sprite will not be able to render
    sf::Texture _spaceshipTex;
    sf::Texture _projectileTex;
    sf::Texture _backgroundTex;
    sf::Texture _bugTex;
    sf::Texture _explosionTex;

    // systems used
    ParallaxSystem _parallaxSystem{_registry};
    DrawSpritesSystem _drawSpritesSystem{_registry};
};



#endif //GAME_HPP
