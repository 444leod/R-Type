/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DeathUpdateSystem.hpp
*/

#ifndef DEATH_UPDATE_SYSTEM_HPP
#define DEATH_UPDATE_SYSTEM_HPP

#include "engine/RestrictedGame.hpp"

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"

#include "../../../Components/Health.hpp"
#include "../../../Components/Monster.hpp"

const Sprite explosionSprite{
    .texture = "assets/orchera/explosion.png",
    .scale = {0.8, 0.8},
    .origin = {128 / 2, 128 / 2},
    .textureRect = sf::IntRect{0, 0, 128, 128}
};

const Animation explosionAnimation{
    .elapsedTime = 0,
    .frameSize = {128, 128},
    .frameDuration = 0.1f,
    .frameCount = 12,
    .loop = false,
    .currentFrame = 0,
    .onEnd = [](Entity entity) {
        // to modify.
    }
};

class DeathUpdateSystem final : public AUpdateSystem
{
public:
    explicit DeathUpdateSystem(ecs::Registry &registry) : AUpdateSystem(registry, "DeathUpdateSystem")
    {
    }

    void execute(const double& deltaTme) override
    {
        _registry.view<Health>().each([&](const Entity& entity, Health& health) {
            if (health.health <= 0)
            {
                if (_registry.has_any_of<Monster>(entity))
                {
                    // _registry.removeComponent<Hitbox>(entity);
                    _registry.get<Hitbox>(entity).shape = shape::Circle{.radius = 0};
                    _registry.removeComponent<Monster>(entity);
                    _registry.removeComponent<Health>(entity);
                    _registry.removeComponent<Sprite>(entity);
                    _registry.removeComponent<Debug>(entity);
                    _registry.addComponent<Sprite>(entity, explosionSprite);

                    auto& anim = _registry.addComponent(entity, explosionAnimation);
                    anim.onEnd = [this, entity] (Entity) {
                        _registry.remove(entity);
                    };
                } else if (_registry.has_any_of<Player>(entity)) {
                    game::RestrictedGame::instance().stop();
                }
            }
        });
    }

private:
};


#endif //DEATH_UPDATE_SYSTEM_HPP
