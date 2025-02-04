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

#include "SharedComponents/Enemy.hpp"
#include <Config.hpp>
#include <Sprites/Level1.hpp>
#include <Utils/IntRect.hpp>

#include <chrono>

class NewProjectileSystem final : public engine::ASystem
{
  public:
    explicit NewProjectileSystem() : ASystem("NewProjectileSystem"), _lastShotTime(0.0) {}

    void execute(const std::uint32_t& shipId, const std::uint32_t& projectileId, const Transform& transform, const std::uint32_t charge)
    {
        const auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count() / 1000.0;

        // Limiter la cadence de tir à 5 tirs par seconde
        if (currentTime - _lastShotTime < 0.2) {
            return;
        }
        _lastShotTime = currentTime;

        try {
            for (auto [entity, ship, sTransform] : _registry.view<Ship, Transform>().each())
            {
                if (ship.id != shipId)
                    continue;
                shape::Rectangle hitboxShape = {};
                const auto projectile = _registry.create();

                auto &sprite = _registry.addComponent(projectile, projectileSprite);
                _registry.addComponent(projectile, transform);
                _registry.addComponent(projectile, Projectile{charge > 20, SCALE * SCREEN_WIDTH + 80, static_cast<std::uint8_t>(projectileId)});
                if (charge <= 20) {
                    hitboxShape = {
                        .width = 80,
                        .height = 50,
                    };
                    sprite.textureRect = IntRect(0, 84, 80, 16);
                    _registry.addComponent(projectile, Animation{.frameSize = {80, 16}, .frameDuration = .030, .frameCount = 3, .loop = false, .onEnd = [this](ecs::Entity entity){
                        if (_registry.has_any_of<Transform>(entity)) {
                            _registry.addComponent(entity, Velocity{.x = 200, .y = 0});
                        }
                    }});
                } else {
                    sprite.textureRect = IntRect(0, 84 + 16 * (charge / 20), 80, 16);
                    hitboxShape = {
                        .width = 40 * static_cast<float>(charge) / 15,
                        .height = 50,
                    };
                    _registry.addComponent(projectile, Animation{.frameSize = {80, 16}, .frameDuration = .050, .frameCount = 2, .loop = true});
                    _registry.addComponent(projectile, Velocity{.x = 200, .y = 0});
                }
                _registry.addComponent(projectile, Hitbox{
                    .shape = hitboxShape,
                    .onCollision = [this](const ecs::Entity entity) {
                        if (_registry.has_all_of<Transform, Enemy>(entity)) {
                            _registry.remove(entity);
                            _points += 100;
                            if (_points >= 1000) {
                                engine::RestrictedGame::instance().scenes().load("win");
                            }
                        }
                    }
                });

                return;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error creating projectile: " << e.what() << std::endl;
        }
    }

  private:
    mutable double _lastShotTime;
    uint32_t _points = 0;
};

#endif /* !NEW_PROJECTILE_SYSTEM_HPP_ */
