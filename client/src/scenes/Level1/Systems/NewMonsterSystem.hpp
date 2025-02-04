/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewMonsterSystem
*/

#ifndef NEW_MONSTER_SYSTEM_HPP_
#define NEW_MONSTER_SYSTEM_HPP_

#include <Engine/Systems/ASystem.hpp>

#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"
#include "PremadeComponents/Projectile.hpp"
#include "SharedComponents/Enemy.hpp"

#include <PremadeComponents/Tags/Debug.hpp>
#include <SharedComponents/Bug.hpp>
#include <Sprites/Level1.hpp>

class NewMonsterSystem final : public engine::ASystem
{
  public:
    explicit NewMonsterSystem() : ASystem("NewMonsterSystem") {}

    void execute(const std::uint32_t& id, const std::uint8_t& type, const Transform& transform, const Velocity& velocity)
    {
      const auto monster = _registry.create();
      _registry.addComponent(monster, bugSprite);
      _registry.addComponent(monster, Enemy{.id = id});
      _registry.addComponent(monster, transform);
      _registry.addComponent(monster, Velocity{.x = -100, .y = 0});
      _registry.addComponent(monster, Bug{});
      _registry.addComponent(monster, Hitbox{
        .shape = shape::Circle{
          .radius = 45,
          .fillColor = {100, 100, 100, 80}
        },
        .onCollision = [this](const ecs::Entity entity) {
          if (_registry.has_any_of<Projectile>(entity)) {
            _registry.remove(entity);
          }
        }
      });
    };

private:
  uint32_t _points = 0;
};

#endif /* !NEW_MONSTER_SYSTEM_HPP_ */
