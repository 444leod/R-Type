/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipShotSystem
*/

#include "ShipShotSystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"

#include "network/UDPPacket.hpp"
#include "Config.hpp"
#include <SFML/Graphics.hpp>
#include "PacketTypes.hpp"
#include "Components.hpp"

std::optional<Entity> getEntityBySource(ecs::Registry &registry, const asio::ip::udp::endpoint &source)
{
    for (auto [entity, info] : registry.view<Client>())
    {
        if (info.info.endpoint == source)
            return entity;
    }
    return std::nullopt;
}

void ShipShotSystem::execute(const asio::ip::udp::endpoint &source) const
{
    const auto entity = getEntityBySource(_registry, source);
    if (!entity.has_value())
            return;

    const auto ship = _registry.

    for (auto [entity, ship, transform] : _registry.view<Ship, Transform>())
    {
        if (ship.id != event.source.id)
            continue;

        constexpr auto projectileVelocity = Velocity{.x = 200, .y = 0};
        const auto shootTransform = Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0};

        static uint32_t projectileId = 0;
        const auto projectile = _registry.create();
        auto projectileSprite = sf::Sprite(_projectileTex);
        projectileSprite.setOrigin(0, 0);
        projectileSprite.setScale(SCALE, SCALE);
        projectileSprite.setPosition(shootTransform.x, shootTransform.y);
        projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        _registry.addComponent(projectile, Hitbox{});
        _registry.addComponent(projectile, projectileSprite);
        _registry.addComponent(projectile, shootTransform);
        _registry.addComponent(projectile, Projectile{ .id = projectileId });
        _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .speed = 20, .frameCount = 3, .loop = false, .velocity = projectileVelocity});
        ntw::UDPPacket packet;
        packet << PACKET_TYPE::NEW_PROJECTILE << event.source.id << projectileId << shootTransform << projectileVelocity;

        // for (const auto &client : CLIENTS)
            // _manager.send(client.endpoint, packet);

        projectileId++;
        return;
    }
}