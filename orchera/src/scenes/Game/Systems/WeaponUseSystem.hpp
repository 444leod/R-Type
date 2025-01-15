/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** WeaponUseSystem.hpp
*/

#ifndef WEAPON_USE_SYSTEM_HPP
#define WEAPON_USE_SYSTEM_HPP

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"
#include "Config.hpp"

#include <cmath>

class PlayerShotSystem final : public ASystem
{
public:
    explicit PlayerShotSystem(ecs::Registry &registry)
        : ASystem(registry, "PlayerShotSystem")
    {
    }

    void execute(const Entity& player, const std::pair<float, float>& mousePos, bool& canShoot)
    {
        const auto&[x, y, z, rotation] = _registry.get<Transform>(player);
        auto& sprite = _registry.get<Sprite>(player);

        const float dx = mousePos.first - x;
        const float dy = mousePos.second - y;
        double angle = std::atan2(dy, dx) * 180 / M_PI;

        std::pair<std::int8_t, std::int8_t> direction = {0, 0};

        for (const auto& [range, dir] : directionMap) {
            if (angle >= range.first && angle < range.second) {
                direction = dir;
                break;
            }
        }

        auto& [frame, directionX] = _directions.at(direction);

        sprite.textureRect = sf::IntRect(frame.first, 192 * 2 + frame.second, 192, 192);
        sprite.scale = {directionX, 1};

        canShoot = false;
        auto&[elapsedTime, frameSize, frameDuration, frameCount, loop, currentFrame, onEnd] = _registry.get<Animation>(player);
        frameCount = 8;
        frameDuration = 0.05f;
        loop = false;
        currentFrame = 0;
        elapsedTime = 0;
        onEnd = [&, angle, x, y] (Entity)
        {
            canShoot = true;
            _registry.addComponent(player, Animation{
                .elapsedTime = 0,
                .frameSize = {192, 192},
                .frameDuration = .20f,
                .frameCount = 6,
                .loop = true,
                .currentFrame = 0,
                .onEnd = [](Entity){}
            });
            auto&[texture, scale, origin, textureRect] = _registry.get<Sprite>(player);
            textureRect = sf::IntRect{0, 0, 192, 192};
            summonArrow(angle, x, y);
        };
    }

private:
    void summonArrow(const float& angle, const float& x, const float& y) const
    {
        const auto arrow = _registry.create();

        _registry.addComponent(arrow, Transform{
            .x = x,
            .y = y,
            .z = 1,
            .rotation = angle
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
            .onCollision = [] (Entity) { //std::cout << "Arrow hit something" << std::endl;
                }
        });

        _registry.addComponent(arrow, Debug{});

        _registry.addComponent(arrow, Projectile{
            .range = 500,
            .id = arrow
        });
    }

    const std::map<std::pair<float, float>, std::pair<std::int8_t, std::int8_t>> directionMap = {
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

#endif //WEAPON_USE_SYSTEM_HPP
