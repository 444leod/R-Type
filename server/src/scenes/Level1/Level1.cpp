/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include <algorithm>
#include <cmath>
#include "Config.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Level1.hpp"
#include "network/NetworkAgent.hpp"
#include "ecs/Registry.hpp"
#include "Structures/UserInput.hpp"
#include "PacketTypes.hpp"

void Level1::initialize() {}

void Level1::update(const double& deltaTime) {
    _executeUpdateSystems(deltaTime);

    // TODO: update this
    // HandleEnemyProjectileCollisionSystem
    // RTYPE SPECIFIC
    //! USES _manager but manager should not be passed to systems

    this->_bugTimer -= deltaTime;
    // if (this->_bugTimer < 0)
    // {
    //     this->_bugTimer = 5.f;
    //     this->addBug(Transform{.x = 2000, .y = 250, .z = 1, .rotation = 90});
    // }

    // auto explosions = _registry.view<Animation, sf::Sprite, Transform>();
    // explosions.displaySets();
}

void Level1::onEnter() {
    _registry.clear();
    const auto spaceship = _registry.create();

    _registry.addComponent(spaceship, spaceshipSprite);
    _registry.addComponent(spaceship, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
    _registry.addComponent(spaceship, Self{});
    _registry.addComponent(spaceship, Hitbox{});
    _registry.addComponent(spaceship, Velocity{.x = 0, .y = 0});
    #if DEBUG
        _registry.addComponent(spaceship, Debug{});
    #endif

    const auto background = _registry.create();

    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{});
}

void Level1::onEnter(const AScene& lastScene)
{
    _registry.clear();

    const auto background = _registry.create();

    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{.offsetMultiplier = 25});
    std::cout << "Game onEnter" << std::endl;
}

void Level1::onExit()
{
}

void Level1::onExit(const AScene& nextScene)
{
}

void Level1::addBug(const Transform& transform) {

    const auto bug = _registry.create();
    _registry.addComponent(bug, bugSprite);
    _registry.addComponent(bug, Bug{});
    _registry.addComponent(bug, Enemy{ .id = _enemyId });
    _registry.addComponent(bug, transform);
    _registry.addComponent(bug, Velocity{.x = -100, .y = 0});
    _registry.addComponent(bug, Hitbox{});
    #if DEBUG
        _registry.addComponent(bug, Debug{});
    #endif

    /* UDPPacket packet;
    packet << PACKET_TYPE::NEW_MONSTER << _enemyId << transform;
    for (auto client: CLIENTS)
        _manager.send(client.endpoint,  packet); */
    _enemyId++;
}
