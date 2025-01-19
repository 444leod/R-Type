/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipShotSystem
*/

#include "ShipShotSystem.hpp"

#include "Network/UDPPacket.hpp"

#include "PremadeComponents/Displayable/Animation.hpp"
#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Projectile.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Client.hpp"
#include "SharedComponents/Ship.hpp"

#include "Config.hpp"
#include "PacketTypes.hpp"

#include <Sprites/Level1.hpp>

static std::optional<ecs::Entity> getEntityBySource(ecs::Registry& registry, const asio::ip::udp::endpoint& source)
{
    for (auto [entity, info] : registry.view<Client>())
    {
        if (info.info.endpoint == source)
            return entity;
    }
    return std::nullopt;
}

void ShipShotSystem::execute(const asio::ip::udp::endpoint& source, std::uint32_t charge) const
{
    const auto entityId = getEntityBySource(_registry, source);
    if (!entityId.has_value())
        return;

    const auto [id] = _registry.get<Ship>(*entityId);
    const auto [x, y, z, rotation] = _registry.get<Transform>(*entityId);

    
    const auto projectile = _registry.create();
    const auto shootTransform = Transform{.x = x + 33 * SCALE, .y = y + 2 * SCALE, .z = 1, .rotation = 0};
    static uint32_t projectileId = 0;

    auto &sprite = _registry.addComponent(projectile, projectileSprite);
    _registry.addComponent(projectile, Transform{.x = x + 80, .y = y, .z = z, .rotation = rotation});
    _registry.addComponent(projectile, Projectile{charge > 20, SCALE * SCREEN_WIDTH + 80, 0});
    if (charge <= 20) {
        sprite.textureRect = IntRect(0, 84, 80, 16);
        _registry.addComponent(projectile, Animation{.frameSize = {80, 16}, .frameDuration = .030, .frameCount = 3, .loop = false, .onEnd = [&](ecs::Entity entity){
            _registry.addComponent(entity, Velocity{.x = 200, .y = 0});
        }});
    } else {
        sprite.textureRect = IntRect(0, 84 + 16 * (charge / 20), 80, 16);
        _registry.addComponent(projectile, Animation{.frameSize = {80, 16}, .frameDuration = .050, .frameCount = 2, .loop = true});
        _registry.addComponent(projectile, Velocity{.x = 200, .y = 0});
    }

    projectileId++;
    ntw::UDPPacket packet;
    packet << PACKET_TYPE::NEW_PROJECTILE << id << projectileId << shootTransform << charge;
    _net->sendPacket(packet);
}
