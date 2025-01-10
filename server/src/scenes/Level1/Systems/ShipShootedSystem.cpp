/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipShootedSystem
*/

#include "ShipShootedSystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"

#include "network/UDPPacket.hpp"
#include "Config.hpp"
#include <SFML/Graphics.hpp>
#include "PacketTypes.hpp"

#include "Sprites/Level1.hpp"

void ShipShootedSystem::execute(const PacketInformation &event, const sf::Texture &_projectileTex) {
    for (auto [entity, ship, transform] : _registry.view<Ship, Transform>())
    {
        if (ship.id != event.source.id)
            continue;

        static uint32_t projectileId = 0;
        const auto projectile = _registry.create();
        const auto shootTransform = Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0};
        _registry.addComponent(projectile, Hitbox{});
        _registry.addComponent(projectile, projectileSprite);
        _registry.addComponent(projectile, shootTransform);
        _registry.addComponent(projectile, Projectile{ .id = projectileId });
        _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .speed = 20, .frameCount = 3, .loop = false, .velocity = Velocity{.x = 200, .y = 0}});
        ntw::UDPPacket packet;
        packet << PACKET_TYPE::NEW_PROJECTILE << event.source.id << projectileId;
        // for (const auto &client : CLIENTS)
            // _manager.send(client.endpoint, packet);
        projectileId++;
    }
}