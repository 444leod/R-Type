/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MonsterUpdateSystem.hpp
*/

#ifndef MONSTER_UPDATE_SYSTEM_HPP
#define MONSTER_UPDATE_SYSTEM_HPP


#include <Engine/Systems/AUpdateSystem.hpp>

#include "PremadeComponents/Displayable/Sprite.hpp"
#include "PremadeComponents/Displayable/Animation.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"
#include "PremadeComponents/Hitbox.hpp"

#include "../../../Components/Monster.hpp"
#include "../../../Components/Health.hpp"

#include "Config.hpp"

const auto goblinSprite = Sprite(
    "assets/orchera/Factions/Goblins/Troops/TNT/Red/TNT_Red.png",
    {1, 1},
    {192 / 2, 192 / 2},
    IntRect{0, 0, 192, 192}
);

const auto goblinAnimation = Animation{
    .elapsedTime = 0,
    .frameSize = {192, 192},
    .frameDuration = 0.20f,
    .frameCount = 6,
    .loop = true,
    .currentFrame = 0,
    .onEnd = [](ecs::Entity) {}
};

const auto torchSprite = Sprite(
    "assets/orchera/Factions/Goblins/Troops/Torch/Red/Torch_Red.png",
    {1, 1},
    {192 / 2, 192 / 2},
    IntRect{0, 0, 192, 192}
);

const auto torchAnimation = Animation{
    .elapsedTime = 0,
    .frameSize = {192, 192},
    .frameDuration = 0.20f,
    .frameCount = 7,
    .loop = true,
    .currentFrame = 0,
    .onEnd = [](ecs::Entity) {}
};

class MonsterUpdateSystem final : public engine::AUpdateSystem
{
public:
    explicit MonsterUpdateSystem() : AUpdateSystem("MonsterUpdateSystem")
    {
    }

    void _execution(double deltaTime) override
    {
        spawnMonster(deltaTime);
        updateVelocity();
    }

private:
    const double spawnDelay = 0.25f;
    double spawnCooldown = 0.f;

    void spawnMonster(const double& deltaTime)
    {
        spawnCooldown -= deltaTime;
        if (spawnCooldown > 0)
            return;

        spawnCooldown = spawnDelay;

        const auto entity = _registry.create();
        const auto randX = std::rand() % (598 - 140) + 70;
        const auto randY = std::rand() % (1012 - (70 + 200)) + 200 ;
        // std::cout << "Spawning monster at " << randX << ", " << randY << std::endl;
        _registry.addComponent(entity, Monster{});
        _registry.addComponent(entity, Transform{
            .x = static_cast<float>(randX),
            .y = static_cast<float>(randY),
            .z = 0,
            .rotation = 0
        });
        _registry.addComponent(entity, Hitbox{
            .shape = shape::Circle{
                .radius = 30,
                .fillColor = {0, 255, 255, 80}
            },
            .onCollision = [](const ecs::Entity&) {}
        });
        // _registry.addComponent(entity, Debug{});
        _registry.addComponent(entity, Health{
            .health = 100,
            .maxHealth = 100
        });

        if (std::rand() % 2 == 0)
        {
            _registry.addComponent(entity, torchSprite);
            _registry.addComponent(entity, torchAnimation);
        } else {
            _registry.addComponent(entity, goblinSprite);
            _registry.addComponent(entity, goblinAnimation);
        }

    }

    void updateVelocity()
    {
        //give a random velocity sometimes to the monster
        _registry.view<Monster, Transform>().each([&](const ecs::Entity& entity, const Monster& monster, Transform &transform) {
            if (std::rand() % 100 == 0)
            {
                _registry.addComponent(entity, Velocity{
                    .x = static_cast<float>(std::rand() % 100 - 50),
                    .y = static_cast<float>(std::rand() % 100 - 50)
                });
            }
        });
    }
};

#endif //MONSTER_UPDATE_SYSTEM_HPP
