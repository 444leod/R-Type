/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MonsterUpdateSystem.hpp
*/

#ifndef MONSTER_UPDATE_SYSTEM_HPP
#define MONSTER_UPDATE_SYSTEM_HPP

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"

#include "BaseComponents.hpp"

#include "../../../Components/Monster.hpp"
#include "../../../Components/Health.hpp"

const auto goblinSprite = Sprite(
    "assets/orchera/Factions/Goblins/Troops/TNT/Red/TNT_Red.png",
    {1, 1},
    {192 / 2, 192 / 2},
    sf::IntRect{0, 0, 192, 192}
);

const auto goblinAnimation = Animation{
    .elapsedTime = 0,
    .frameSize = {192, 192},
    .frameDuration = 0.20f,
    .frameCount = 6,
    .loop = true,
    .currentFrame = 0,
    .onEnd = [](Entity) {}
};

const auto torchSprite = Sprite(
    "assets/orchera/Factions/Goblins/Troops/Torch/Red/Torch_Red.png",
    {1, 1},
    {192 / 2, 192 / 2},
    sf::IntRect{0, 0, 192, 192}
);

const auto torchAnimation = Animation{
    .elapsedTime = 0,
    .frameSize = {192, 192},
    .frameDuration = 0.20f,
    .frameCount = 7,
    .loop = true,
    .currentFrame = 0,
    .onEnd = [](Entity) {}
};

class MonsterUpdateSystem final : public AUpdateSystem
{
public:
    explicit MonsterUpdateSystem(ecs::Registry &registry) : AUpdateSystem(registry, "MonsterUpdateSystem")
    {
    }

    void execute(const double& deltaTime) override
    {
        spawnMonster(deltaTime);
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
            .onCollision = [](const Entity&) {}
        });
        // _registry.addComponent(entity, Debug{});
        _registry.addComponent(entity, Health{
            .health = 1,
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
};

#endif //MONSTER_UPDATE_SYSTEM_HPP
