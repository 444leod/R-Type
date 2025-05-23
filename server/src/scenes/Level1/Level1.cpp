/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Level1.hpp"

#include "Config.hpp"

#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Tags/Self.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Bug.hpp"
#include "SharedComponents/Enemy.hpp"
#include "SharedComponents/Parallax.hpp"

#include <PremadeModules/Network/ANetworkSceneModule.hpp>
#include <SharedComponents/Client.hpp>
#include <SharedComponents/Ship.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

void Level1::initialize() {}

void Level1::update(const double& deltaTime)
{
    _executeUpdateSystems(deltaTime);
    if (_networkModule == nullptr)
        return;
    // std::cout << "clients: " << _networkModule->clients().size() << std::endl;
    if (_networkModule->clients().empty())
        engine::RestrictedGame::instance().stop();
}

void Level1::onEnter()
{
    _networkModule = this->getModule<ANetworkSceneModule>();
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

    const auto net = this->getModule<ANetworkSceneModule>();
    if (net == nullptr)
    {
        std::cerr << "No network module found, exiting..." << std::endl;
        throw std::runtime_error("No network module found");
    }

    for (auto& client : net->clients())
    {
        const auto spaceship = _registry.create();

        std::cout << "spaceship: " << spaceship << std::endl;

        _registry.addComponent(spaceship, spaceshipSprite);
        _registry.addComponent(spaceship, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
        _registry.addComponent(spaceship, Hitbox{});
        _registry.addComponent(spaceship, Velocity{.x = 0, .y = 0});
        _registry.addComponent(spaceship, Ship{.id = client.id});
        _registry.addComponent(spaceship, Client{client});

        ntw::UDPPacket packet;
        packet << PACKET_TYPE::YOUR_SHIP << client.id << _registry.get<Transform>(spaceship) << _registry.get<Velocity>(spaceship);
        net->sendPacket(client.endpoint, packet);

        ntw::UDPPacket broadcast;
        broadcast << PACKET_TYPE::NEW_SHIP << client.id << _registry.get<Transform>(spaceship) << _registry.get<Velocity>(spaceship);
        for (const auto& otherClient : net->clients())
        {
            if (otherClient.id == client.id)
                continue;
            net->sendPacket(otherClient.endpoint, broadcast);
        }
    }
}

void Level1::onEnter(const AScene& lastScene)
{
    _networkModule = this->getModule<ANetworkSceneModule>();
    _registry.clear();

    const auto background = _registry.create();

    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{.offsetMultiplier = 25});

    const auto net = this->getModule<ANetworkSceneModule>();
    if (net == nullptr)
    {
        std::cerr << "No network module found, exiting..." << std::endl;
        throw std::runtime_error("No network module found");
    }

    for (auto& client : net->clients())
    {
        const auto spaceship = _registry.create();

        std::cout << "spaceship: " << spaceship << std::endl;

        _registry.addComponent(spaceship, spaceshipSprite);
        _registry.addComponent(spaceship, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
        _registry.addComponent(spaceship, Hitbox{});
        _registry.addComponent(spaceship, Velocity{.x = 0, .y = 0});
        _registry.addComponent(spaceship, Ship{.id = client.id});
        _registry.addComponent(spaceship, Client{client});

        ntw::UDPPacket packet;
        packet << PACKET_TYPE::YOUR_SHIP << client.id << _registry.get<Transform>(spaceship) << _registry.get<Velocity>(spaceship);
        net->sendPacket(client.endpoint, packet);
        std::cout << "sent ship packet of size " << packet.size() << std::endl;

        ntw::UDPPacket broadcast;
        broadcast << PACKET_TYPE::NEW_SHIP << client.id << _registry.get<Transform>(spaceship) << _registry.get<Velocity>(spaceship);
        for (const auto& otherClient : net->clients())
        {
            if (otherClient.id == client.id)
                continue;
            net->sendPacket(otherClient.endpoint, broadcast);
        }
    }
}

void Level1::onExit() {}

void Level1::onExit(const AScene& nextScene) {}

void Level1::addBug(const Transform& transform)
{

    const auto bug = _registry.create();
    _registry.addComponent(bug, bugSprite);
    _registry.addComponent(bug, Bug{});
    _registry.addComponent(bug, Enemy{.id = _enemyId});
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
