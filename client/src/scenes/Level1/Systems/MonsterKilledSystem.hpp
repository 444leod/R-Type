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
#include <config.h>
#include "../Events/PacketInformations.hpp"
#include <SFML/Graphics.hpp>

class MonsterKilledSystem final : public ASystem
{
public:
    MonsterKilledSystem(Registry &registry, const std::string name = "MonsterKilledSystem") : ASystem(registry, name) {}

    void execute(const PacketInformations &event, const sf::Texture &explosionTex) {
        uint32_t monster_id;
        uint32_t projectile_id;
        Transform transform;
        event.packet >> monster_id >> projectile_id;

        _registry.view<Enemy, sf::Sprite, Transform>().each([&](const Entity& enemy, const Enemy&e_enemy, const sf::Sprite& sprite, const Transform& transform)  {
            _registry.view<Projectile, Transform>().each([&](const Entity& projectile, const Projectile&p_projectile, const Transform& projectile_transform) {
                if (e_enemy.id != monster_id || p_projectile.id != projectile_id)
                    return;
                const auto explosion = _registry.create();
                auto explosionSprite = sf::Sprite(explosionTex);
                explosionSprite.setOrigin(16, 16);
                explosionSprite.setScale(SCALE, SCALE);
                explosionSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
                explosionSprite.setPosition(transform.x, transform.y);
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
