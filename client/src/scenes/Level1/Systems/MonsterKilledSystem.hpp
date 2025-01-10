/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MonsterKilledSystem
*/

#ifndef MONSTERKILLEDSYSTEM_HPP_
#define MONSTERKILLEDSYSTEM_HPP_


#include "BaseSystems/Abstracts/ASystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"
#include "Config.hpp"

#include "../Events/PacketInformation.hpp"
#include <SFML/Graphics.hpp>

class MonsterKilledSystem final : public ASystem
{
public:
    explicit MonsterKilledSystem(ecs::Registry &registry) : ASystem(registry, "MonsterKilledSystem") {}

    void execute(const PacketInformation &event, const sf::Texture &explosionTex) const {
        uint32_t monster_id;
        uint32_t projectile_id;
        event.packet >> monster_id >> projectile_id;

        _registry.view<Enemy, Sprite, Transform>().each([&](const Entity& enemy, const Enemy&e_enemy, const Sprite&, const Transform& transform)  {
            _registry.view<Projectile, Transform>().each([&](const Entity& projectile, const Projectile&p_projectile, const Transform& projectile_transform) {
                if (e_enemy.id != monster_id || p_projectile.id != projectile_id)
                    return;
                const auto explosion = _registry.create();
                _registry.addComponent(explosion, explosionSprite);
                _registry.addComponent(explosion, Transform{.x = transform.x, .y = transform.y, .z = 1, .rotation = 0});
                _registry.addComponent(explosion, Animation{.frameSize = {32, 32}, .speed = 100, .frameCount = 6, .loop = false});
                _registry.remove(enemy);
                _registry.remove(projectile);
            });
        });
    }
};

#endif /* !MONSTERKILLEDSYSTEM_HPP_ */
