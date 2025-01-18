/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewProjectileSystem
*/

#ifndef NEW_PROJECTILE_SYSTEM_HPP_
#define NEW_PROJECTILE_SYSTEM_HPP_

#include <Engine/Systems/ASystem.hpp>

#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"
#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Projectile.hpp"
#include "PremadeComponents/Displayable/Animation.hpp"

class NewProjectileSystem final : public engine::ASystem
{
  public:
    explicit NewProjectileSystem() : ASystem("NewProjectileSystem") {}

    void execute(const std::uint32_t& shipId, const std::uint32_t& projectileId, const Transform& transform, const Velocity& velocity) const
    {
        for (auto [_, ship, transform] : _registry.view<Ship, Transform>().each())
        {
            if (ship.id != shipId)
                continue;
            const auto projectile = _registry.create();

            // TODO: update

            // auto projectileSprite = sf::Sprite(projectileTex);
            // projectileSprite.setOrigin(0, 0);
            // projectileSprite.setScale(SCALE, SCALE);
            // projectileSprite.setPosition(shootTransform.x, shootTransform.y);
            // projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
            // _registry.addComponent(projectile, projectileSprite);

            _registry.addComponent(projectile, Hitbox{});
            _registry.addComponent(projectile, transform);
            _registry.addComponent(projectile, Projectile{.id = projectileId});
            _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .frameDuration = 0.02, .frameCount = 3, .loop = false, .onEnd = [&](const ecs::Entity& entity) { _registry.addComponent(entity, Velocity{.x = 200, .y = 0}); }});
            return;
        }
    }
};

#endif /* !NEW_PROJECTILE_SYSTEM_HPP_ */
