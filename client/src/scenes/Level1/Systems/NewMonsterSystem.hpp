/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewMonsterSystem
*/

#ifndef NEW_MONSTER_SYSTEM_HPP_
#define NEW_MONSTER_SYSTEM_HPP_

#include "PremadeSystems/Abstracts/ASystem.hpp"

#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Enemy.hpp"

class NewMonsterSystem final : public ASystem {
  public:
    explicit NewMonsterSystem() : ASystem("NewMonsterSystem") {}

    void execute(const std::uint32_t& id, const std::uint8_t& type, const Transform& transform, const Velocity& velocity) const {
        const auto monster = _registry.create();
        //        auto monsterSprite = sf::Sprite(_bugTex);
        //        monsterSprite.setOrigin(16, 13);
        //        monsterSprite.setScale(SCALE, SCALE);
        //        monsterSprite.setPosition(transform.x, transform.y);
        _registry.addComponent(monster, Enemy{.id = id});
        //        _registry.addComponent(monster, Monster{.type = type});
        _registry.addComponent(monster, transform);
        _registry.addComponent(monster, velocity);
        _registry.addComponent(monster, Hitbox{});
    }
};

#endif /* !NEW_MONSTER_SYSTEM_HPP_ */
