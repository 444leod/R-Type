/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewProjectileSystem
*/

#ifndef NEW_PROJECTILE_SYSTEM_HPP_
#define NEW_PROJECTILE_SYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"
#include "Config.hpp"
#include "../Events/PacketInformation.hpp"

#include "Sprites/Level1.hpp"

class NewProjectileSystem final : public ASystem
{
public:
    explicit NewProjectileSystem(ecs::Registry &registry) : ASystem(registry, "NewProjectileSystem") {}

    void execute(const PacketInformation &event, const sf::Texture &projectileTex) const {
        std::uint32_t projectileId; 
        std::uint32_t shipId;
        event.packet >> shipId >> projectileId;
        for (auto [entity, ship, transform] : _registry.view<Ship, Transform>().each())
        {
            if (ship.id != shipId)
                continue;
            const auto projectile = _registry.create();
            const auto shootTransform = Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0};
            _registry.addComponent(projectile, Hitbox{});
            _registry.addComponent(projectile, projectileSprite);
            _registry.addComponent(projectile, shootTransform);
            _registry.addComponent(projectile, Projectile{ .id = projectileId });
            _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .speed = 20, .frameCount = 3, .loop = false, .velocity = Velocity{.x = 200, .y = 0}});
        }
    }
};


#endif /* !NEW_PROJECTILE_SYSTEM_HPP_ */
