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
      _registry.addComponent(spaceship, Hitbox{});

      _registry.addComponent(spaceship, spaceshipSprite);
#if DEBUG
      _registry.addComponent(spaceship, Debug{});
#endif

        return spaceship;
    }
};

#endif /* !NEW_SHIP_SYSTEM_HPP_ */
