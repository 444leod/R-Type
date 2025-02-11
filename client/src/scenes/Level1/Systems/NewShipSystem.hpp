/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewShipSystem
*/

#ifndef NEW_SHIP_SYSTEM_HPP_
#define NEW_SHIP_SYSTEM_HPP_

#include <Engine/Systems/ASystem.hpp>

#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Ship.hpp"
#include "SharedComponents/Enemy.hpp"

#include <PremadeComponents/Tags/Self.hpp>

class NewShipSystem final : public engine::ASystem
{
  public:
    explicit NewShipSystem() : ASystem("NewShipSystem") {}

    ecs::Entity execute(const std::uint32_t& id, const Transform& transform, const Velocity& velocity) const
    {
      const auto spaceship = _registry.create();
      _registry.addComponent(spaceship, Ship{.id = id});
      _registry.addComponent(spaceship, transform);
      _registry.addComponent(spaceship, velocity);
    #if DEBUG
      _registry.addComponent(spaceship, Debug{});
    #endif
      _registry.addComponent(spaceship, Hitbox{
        .shape = shape::Rectangle{
            .width = 150,
            .height = 80,
            .fillColor = { 255, 0, 0, 150}
        },
        .onCollision = [this](const ecs::Entity& entity)
        {
          if (_registry.has_any_of<Enemy>(entity))
          {
            engine::RestrictedGame::instance().scenes().load("lose");
          }
        }
      });

      auto &sprite = _registry.addComponent(spaceship, spaceshipSprite);
      sprite.textureRect = IntRect(0, id * 16, 32, 16);
#if DEBUG
      _registry.addComponent(spaceship, Debug{});
#endif

        return spaceship;
    }
};

#endif /* !NEW_SHIP_SYSTEM_HPP_ */
