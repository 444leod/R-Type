/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewProjectileSystem
*/

#ifndef NEW_PROJECTILE_SYSTEM_HPP_
#define NEW_PROJECTILE_SYSTEM_HPP_

#include <Engine/Systems/ASystem.hpp>

#include "PremadeComponents/Displayable/Animation.hpp"
#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Projectile.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"
#include "SharedComponents/Ship.hpp"

#include <Config.hpp>
#include <Sprites/Level1.hpp>
#include <Utils/IntRect.hpp>

class NewProjectileSystem final : public engine::ASystem
{
  public:
    explicit NewProjectileSystem() : ASystem("NewProjectileSystem") {}

    void execute(const std::uint32_t& shipId, const std::uint32_t& projectileId, const Transform& transform, const Velocity& velocity, const short charge) const
    {
        for (auto [_, ship, transform] : _registry.view<Ship, Transform>().each())
        {
            if (ship.id != shipId)
                continue;
            const auto projectile = _registry.create();

            auto &sprite = _registry.addComponent(projectile, projectileSprite);
            _registry.addComponent(projectile, transform);
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
            return;
        }
    }
};

#endif /* !NEW_PROJECTILE_SYSTEM_HPP_ */
