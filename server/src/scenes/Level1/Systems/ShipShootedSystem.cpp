/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipShootedSystem
*/

#include "ShipShootedSystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"

#include "UDPPacket.hpp"
#include "Global.hpp"
#include "config.h"
#include <SFML/Graphics.hpp>

void ShipShootedSystem::execute(const PacketInformations &event, const sf::Texture &_projectileTex) {
    for (auto [shipEntity, ship, transform] : _registry.view<Ship, Transform>())
    {
        if (ship.id != event.source.id)
            continue;

        static uint32_t projectileId = 0;
        const auto projectile = _registry.create();
        auto projectileSprite = sf::Sprite(_projectileTex);
        const auto shootTransform = Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0};
        projectileSprite.setOrigin(0, 0);
        projectileSprite.setScale(SCALE, SCALE);
        projectileSprite.setPosition(shootTransform.x, shootTransform.y);
        projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        _registry.addComponent(projectile, Hitbox{});
        _registry.addComponent(projectile, projectileSprite);
        _registry.addComponent(projectile, shootTransform);
        _registry.addComponent(projectile, Projectile{ .id = projectileId });
        _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .frameDuration = 0.02, .frameCount = 3, .loop = false, .onEnd = [&](Entity entity){
            _registry.addComponent(entity, Velocity{.x = 200, .y = 0});
        }});
        UDPPacket packet;
        packet << PACKET_TYPE::NEW_PROJECTILE << event.source.id << projectileId;
        for (const auto &client : CLIENTS)
            _manager.send(client.endpoint, packet);
        projectileId++;
    }
}