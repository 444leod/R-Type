/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AttackSystem.hpp
*/

#ifndef ATTACK_SYSTEM_HPP
#define ATTACK_SYSTEM_HPP

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"
#include "Config.hpp"

#include "../../../Components/Weapon.hpp"
#include "../../../Components/Target.hpp"
#include "../../../Components/Monster.hpp"
#include "../../../Components/Health.hpp"

#include <cmath>

const Sprite arrowSprite(
    "assets/orchera/Factions/Knights/Troops/Archer/Arrow/Arrow.png",
    {1, 1},
    {64 / 2, 64 / 2},
    sf::IntRect{0, 0, 64, 64}
);

class AttackSystem final : public AUpdateSystem
{
public:
    explicit AttackSystem(ecs::Registry &registry) : AUpdateSystem(registry, "AttackSystem")
    {
    }

    void execute(const double& deltaTime)
    {
        _registry.view<Transform, Weapon, Target>().each([&](const Entity& entity, const Transform& transform, Weapon& weapon, const Target& target) {
            std::visit([&](auto& weaponType) {

                weaponType.shootCooldown -= deltaTime;

                if (weaponType.shootCooldown <= 0) {
                    if (!_registry.has_any_of<Transform>(target.target))
                        return;

                    const auto targetCoords = _registry.get<Transform>(target.target);

                    const float dx = targetCoords.x - transform.x;
                    const float dy = targetCoords.y - transform.y;
                    const double angle = std::atan2(dy, dx) * 180 / M_PI;

                    std::pair<std::int8_t, std::int8_t> direction = {0, 0};

                    for (const auto& [range, dir] : _directionMap) {
                        if (angle >= range.first && angle < range.second) {
                            direction = dir;
                            break;
                        }
                    }

                    auto& [frame, directionX] = _directions.at(direction);

                    attack(weaponType, entity, transform, {targetCoords.x, targetCoords.y}, directionX, frame, angle);
                    weaponType.shootCooldown = weaponType.shootDelay;
                }
            }, weapon.weapon);
        });
    }

private:
    void attack(Bow& bow, const Entity& player, const Transform& source, const std::pair<float, float>& target, const int directionX, const std::pair<int, int>& frame, const double angle)
    {
        auto& sprite = _registry.get<Sprite>(player);

        sprite.textureRect = sf::IntRect(frame.first, 192 * 2 + frame.second, 192, 192);
        sprite.scale = {directionX, 1};

        if (!_registry.has_any_of<Animation>(player))
            _registry.addComponent(player, Animation{});

        auto&[elapsedTime, frameSize, frameDuration, frameCount, loop, currentFrame, onEnd] = _registry.get<Animation>(player);
        frameCount = 8;
        frameDuration = bow.attackSpeed / 8;
        loop = false;
        currentFrame = 0;
        elapsedTime = 0;
        onEnd = [&, angle, source] (Entity) {
            const auto arrow = _registry.create();

            _registry.addComponent(arrow, Transform{
                .x = source.x,
                .y = source.y,
                .z = 1,
                .rotation = static_cast<float>(angle)
            });

            _registry.addComponent(arrow, arrowSprite);
            _registry.addComponent(arrow, Velocity{
                .x = static_cast<float>(std::cos(angle * M_PI / 180.0f)) * 500,
                .y = static_cast<float>(std::sin(angle * M_PI / 180.0f)) * 500
            });

            _registry.addComponent(arrow, Hitbox{
                .shape = shape::Circle{
                    .radius = 10,
                    .fillColor = {255, 100, 100, 255}
                },
                .onCollision = [this, &bow, arrow] (const Entity& entity) {
                    if (_registry.has_any_of<Monster>(entity))
                    {
                        auto& proj = _registry.get<Projectile>(arrow);
                        auto& health = _registry.get<Health>(entity);
                        health.health -= proj.damage;
                        if (proj.pierce > 0)
                            proj.pierce--;
                        else
                            _registry.remove(arrow);
                    }
                }
            });

            // _registry.addComponent(arrow, Debug{});

            _registry.addComponent(arrow, Projectile{
                .range = bow.range,
                .pierce = bow.pierce,
                .damage = bow.damage,
                .id = arrow,
            });
        };
    }

    const std::map<std::pair<float, float>, std::pair<std::int8_t, std::int8_t>> _directionMap = {
        { { -22.5, 22.5 }, {1, 0} },
       { { 22.5, 67.5 }, {1, 1} },
       { { 67.5, 112.5 }, {0, 1} },
       { { 112.5, 157.5 }, {-1, 1} },
       { { -67.5, -22.5 }, {1, -1} },
       { { -112.5, -67.5 }, {0, -1} },
       { { -157.5, -112.5 }, {-1, -1} },
       { { -180.0, -157.5 }, {-1, 0} },
       { { 157.5, 180.0 }, {-1, 0} }
    };

    const std::map<std::pair<std::int8_t, std::int8_t>, std::pair<std::pair<int, int>, int>> _directions = {
        {{0, -1}, {{0, 0}, 1}},
        {{1, -1}, {{0, 192}, 1}},
        {{1, 0}, {{0, 192 * 2}, 1}},
        {{1, 1}, {{0, 192 * 3}, 1}},
        {{0, 1}, {{0, 192 * 4}, 1}},
        {{0, 0}, {{0, 192 * 4}, 1}},
        {{-1, 1}, {{0, 192 * 3}, -1}},
        {{-1, 0}, {{0, 192 * 2}, -1}},
        {{-1, -1}, {{0, 192}, -1}}
    };
};

#endif //ATTACK_SYSTEM_HPP
