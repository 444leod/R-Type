/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MonsterKilledSystem
*/

#ifndef MONSTER_KILLED_SYSTEM_HPP_
#define MONSTER_KILLED_SYSTEM_HPP_

#include <ECS/Entity.hpp>

#include "PremadeSystems/Abstracts/ASystem.hpp"

#include "PremadeComponents/Displayable/Animation.hpp"
#include "PremadeComponents/Projectile.hpp"
#include "PremadeComponents/Transform.hpp"

#include "SharedComponents/Enemy.hpp"

class MonsterKilledSystem final : public ASystem
{
  public:
    explicit MonsterKilledSystem() : ASystem("MonsterKilledSystem") {}

    void execute(const std::uint32_t& monsterId, const std::uint32_t& projectileId) const
    {
        std::optional<std::tuple<ecs::Entity, Enemy, Transform>> monster = std::nullopt;

        for (auto& [entity, enemy, transform] : _registry.view<Enemy, Transform>().each())
        {
            if (enemy.id != monsterId)
                continue;

            monster = std::make_tuple(entity, enemy, transform);
            break;
        }

        if (!monster.has_value())
            return;

        std::optional<std::tuple<ecs::Entity, Projectile, Transform>> projectile = std::nullopt;

        for (auto& [entity, proj, transform] : _registry.view<Projectile, Transform>().each())
        {
            if (proj.id != projectileId)
                continue;

            projectile = std::make_tuple(entity, proj, transform);
            break;
        }

        if (!projectile.has_value())
            return;

        const auto [monsterEntity, monsterEnemy, monsterTransform] = *monster;
        const auto [projectileEntity, projectileProjectile, projectileTransform] = *projectile;
        _registry.remove(monsterEntity);
        _registry.remove(projectileEntity);

        const auto explosion = _registry.create();

        // auto explosionSprite = sf::Sprite(explosionTex);
        // explosionSprite.setOrigin(16, 16);
        // explosionSprite.setScale(SCALE, SCALE);
        // explosionSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
        // explosionSprite.setPosition(monsterTransform->x, monsterTransform->y);
        // _registry.addComponent(explosion, explosionSprite);

        _registry.addComponent(explosion, Transform{.x = monsterTransform.x, .y = monsterTransform.y, .z = 1, .rotation = 0});
        _registry.addComponent(explosion, Animation{.frameSize = {32, 32}, .frameDuration = 0.1, .frameCount = 6, .loop = false, .onEnd = [&](const ecs::Entity& entity) { _registry.remove(entity); }});
    }
};

#endif /* !MONSTER_KILLED_SYSTEM_HPP_ */
