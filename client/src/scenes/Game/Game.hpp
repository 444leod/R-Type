/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef GAME_HPP
#define GAME_HPP

#include "AScene.hpp"
#include "Registry.hpp"
#include "EventDispatcher.hpp"
#include "Components.hpp"
#include "Events/PlayerMovement.hpp"
#include <chrono>

class Game final : public AScene {
public:
    Game(ISceneManager& m, const std::string& n) : AScene(m, n) {
        _spaceshipTex.loadFromFile("assets/r-typesheet42.gif", sf::IntRect(0, 0, 34, 18));
        _projectileTex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(0, 91, 48, 16));
        _backgroundTex.loadFromFile("assets/rtype-background.png", sf::IntRect(0, 243, 3072, 205));
        _bugTex.loadFromFile("assets/r-typesheet8.gif");
        _explosionTex.loadFromFile("assets/r-typesheet44.gif", sf::IntRect(131, 0, 192, 32));
        _eventDispatcher.link<movement_event>(&_playerMovement);
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
    void addProjectile(const Transform& transform);
    void addBug(const Transform& transform);

public:
private:
    Registry _registry;
    float _parallaxOffset = 0;
    EventDispatcher _eventDispatcher;

    PlayerMovement _playerMovement{_registry};

    //this is temporary, if this texture get deleted, then the sprite will not be able to render
    sf::Texture _spaceshipTex;
    sf::Texture _projectileTex;
    sf::Texture _backgroundTex;
    sf::Texture _bugTex;
    sf::Texture _explosionTex;
};



#endif //GAME_HPP
