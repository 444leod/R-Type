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

static std::optional<ecs::Entity> getEntityBySource(ecs::Registry& registry, const asio::ip::udp::endpoint& source) {
    for (auto [entity, info] : registry.view<Client>()) {
        if (info.info.endpoint == source)
            return entity;
    }
    return std::nullopt;
}

void ShipShotSystem::execute(const asio::ip::udp::endpoint& source) const {
    const auto entityId = getEntityBySource(_registry, source);
    if (!entityId.has_value())
        return;

    const auto [id] = _registry.get<Ship>(*entityId);
    const auto [x, y, z, rotation] = _registry.get<Transform>(*entityId);

    constexpr auto projectileVelocity = Velocity{.x = 200, .y = 0};
    const auto shootTransform = Transform{.x = x + 33 * SCALE, .y = y + 2 * SCALE, .z = 1, .rotation = 0};

    static uint32_t projectileId = 0;
    const auto projectile = _registry.create();

    // auto projectileSprite = sf::Sprite(_projectileTex);
    // projectileSprite.setOrigin(0, 0);
    // projectileSprite.setScale(SCALE, SCALE);
    // projectileSprite.setPosition(shootTransform.x, shootTransform.y);
    // projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    // _registry.addComponent(projectile, projectileSprite);

    _registry.addComponent(projectile, Hitbox{});
    _registry.addComponent(projectile, shootTransform);
    _registry.addComponent(projectile, Projectile{.id = projectileId++});
    _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .frameDuration = 0.02, .frameCount = 3, .loop = false, .onEnd = [&](const ecs::Entity& entity) { _registry.addComponent(entity, Velocity{.x = 200, .y = 0}); }});

    ntw::UDPPacket packet;
    packet << PACKET_TYPE::NEW_PROJECTILE << id << projectileId << shootTransform << projectileVelocity;
    _net->sendPacket(packet);
}