/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Level1.hpp"

#include "Config.hpp"

#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"
#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Tags/Self.hpp"

#include "SharedComponents/Enemy.hpp"
#include "SharedComponents/Bug.hpp"
#include "SharedComponents/Parallax.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

void Level1::initialize() {}

void Level1::update(const double& deltaTime) {
    _executeUpdateSystems(deltaTime);
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
