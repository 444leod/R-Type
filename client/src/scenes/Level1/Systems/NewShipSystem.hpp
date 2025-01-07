/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewShipSystem
*/

#ifndef NEW_SHIP_SYSTEM_HPP_
#define NEW_SHIP_SYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"
#include "Config.hpp"

//#include <SFML/Graphics.hpp>

class NewShipSystem final : public ASystem
{
public:
    explicit NewShipSystem(ecs::Registry &registry) : ASystem(registry, "NewShipSystem") {}

    Entity execute(const std::uint32_t& id, const Transform& transform, const Velocity& velocity) {
        const auto spaceship = _registry.create();
        _registry.addComponent(spaceship, Ship{.id = id});
        _registry.addComponent(spaceship, transform);
        _registry.addComponent(spaceship, velocity);
        _registry.addComponent(spaceship, Hitbox{});

        //TODO: update
//        auto spaceshipSprite = sf::Sprite(_spaceshipTex);
//        spaceshipSprite.setOrigin(0, 0);
//        spaceshipSprite.setScale(SCALE, SCALE);
//        spaceshipSprite.setPosition(transform.x, transform.y);

        return spaceship;
    }
};


#endif /* !NEW_SHIP_SYSTEM_HPP_ */
