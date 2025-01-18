/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Level1.hpp"

#include "PremadeModules/Network/ANetworkGameModule.hpp"
#include "PremadeModules/Network/ANetworkSceneModule.hpp"

#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Projectile.hpp"
#include "PremadeComponents/Velocity.hpp"
#include "PremadeComponents/Displayable/Animation.hpp"

#include "SharedComponents/Enemy.hpp"

#include "Sprites/Level1.hpp"
#include "Config.hpp"
#include "PacketTypes.hpp"

void Level1::initialize()
{
}

void Level1::update(const double& deltaTime)
{
    _executeUpdateSystems(deltaTime);
}

void Level1::onEnter()
{
    _registry.clear();

    const auto background = _registry.create();

    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{.offsetMultiplier = 25});
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
    ntw::UDPPacket packet;
    packet << PACKET_TYPE::DISCONNECT;

    const auto net = this->getModule<ANetworkSceneModule>();
    if (net == nullptr)
        return;
    net->sendPacket(packet);
}

void Level1::onExit(const AScene& nextScene)
{
}

void Level1::addProjectile(const Transform& transform){
    const auto projectile = _registry.create();

    _registry.addComponent(projectile, projectileSprite);
    // _registry.addComponent(projectile, Hitbox{});
    _registry.addComponent(projectile, transform);
    _registry.addComponent(projectile, Projectile{});
    _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .frameDuration = .020, .frameCount = 3, .loop = false, .onEnd = [&](const ecs::Entity& entity){
        _registry.addComponent(entity, Velocity{.x = 200, .y = 0});
    }});
    #if DEBUG
        _registry.addComponent(projectile, Debug{});
    #endif
}

void Level1::addBug(const Transform& transform) {
    const auto bug = _registry.create();
    _registry.addComponent(bug, bugSprite);
    _registry.addComponent(bug, Bug{});
    _registry.addComponent(bug, Enemy{});
    _registry.addComponent(bug, transform);
    _registry.addComponent(bug, Velocity{.x = -100, .y = 0});
    _registry.addComponent(bug, Hitbox{});
    #if DEBUG
        _registry.addComponent(bug, Debug{});
    #endif
}
