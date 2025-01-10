/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef LEVEL_1_HPP
#define LEVEL_1_HPP

#include <chrono>
#include <memory>

#include "engine/AScene.hpp"
#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"

// From Game Engine
#include "BaseSystems/Update/ParalaxSystem.hpp"
#include "BaseSystems/Update/MovementSystem.hpp"
#include "BaseSystems/Update/AnimateSystem.hpp"
#include "BaseSystems/Render/DrawSpritesSystem.hpp"

// Level1 specific
#include "Events/InputHandler.hpp"
#include "Events/PacketHandler.hpp"
#include "Systems/RemoveOutOfBoundProjectilesSystem.hpp"
#include "Systems/BugsMovementSystem.hpp"

#include <chrono>
#include <memory>

#include "Structures/UserInput.hpp"

class Level1 final : public AScene {
public:
    Level1(RestrictedSceneManager& m, ecs::Registry& r, const std::string& n) : AScene(m, r, n) {
        _spaceshipTex.loadFromFile("assets/r-typesheet42.gif", sf::IntRect(0, 0, 34, 18));
        _projectileTex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(0, 91, 48, 16));
        _backgroundTex.loadFromFile("assets/rtype-background.png", sf::IntRect(0, 243, 3072, 205));
        _bugTex.loadFromFile("assets/r-typesheet8.gif");
        _explosionTex.loadFromFile("assets/r-typesheet44.gif", sf::IntRect(131, 0, 192, 32));
        _eventDispatcher.link<UserInput>(&_inputHandler);
        _eventDispatcher.link<PacketInformation>(&_packetHandler);
        // systems
        // update systems
        this->_updateSystems.push_back(std::make_unique<ParallaxSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<MovementSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<RemoveOutOfBoundProjectilesSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<AnimateSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<BugsMovementSystem>(_registry));
        // render systems
        this->_renderSystems.push_back(std::make_unique<DrawSpritesSystem>(_registry));
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

private:
    void addProjectile(const Transform& transform);
    void addBug(const Transform& transform);
    // void _handleYourShip(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet);
    // void _handleNewShip(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet);
    // void _handleDisconnect(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet);
    // void _handleShipMovement(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet);
    // void _handleNewProjectile(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet);
    // void _handleNewMonster(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet);
    // void _handleMonsterKilled(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet);


public:
private:
    ecs::EventDispatcher _eventDispatcher;

    asio::ip::udp::endpoint _server{};

    InputHandler _inputHandler{_registry, _manager};
    PacketHandler _packetHandler{_registry, _manager};

    std::vector<ntw::ClientInformation> _clients;

    //this is temporary, if this texture get deleted, then the sprite will not be able to render
    sf::Texture _spaceshipTex;
    sf::Texture _projectileTex;
    sf::Texture _backgroundTex;
    sf::Texture _bugTex;
    sf::Texture _explosionTex;
};

#endif //GAME_HPP
