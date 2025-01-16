/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PlayerUpdateSystem.hpp
*/

#ifndef PLAYER_UPDATE_SYSTEM_HPP
#define PLAYER_UPDATE_SYSTEM_HPP

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"

#include "BaseComponents.hpp"

#include "../../../Components/Monster.hpp"
#include "../../../Components/Target.hpp"

#include <cmath>

class PlayerUpdateSystem final : public AUpdateSystem
{
public:
    explicit PlayerUpdateSystem(ecs::Registry &registry, const Entity& player)
        : AUpdateSystem(registry, "PlayerUpdateSystem"), player(player)
    {
    }

    void execute(const double& deltaTime) override
    {
        updateAnimation();
        attack();
    }

private:
    Entity player;

    void attack()
    {
        const auto velocity = _registry.get<Velocity>(player);
        if (velocity.x != 0 || velocity.y != 0)
        {
            _registry.removeComponent<Target>(player);
            return;
        }

        const auto [x, y, z, rotation] = _registry.get<Transform>(player);
        std::optional<std::pair<Entity, double>> target = std::nullopt;
        for (const auto& [monster, _, monsterPos] : _registry.view<Monster, Transform>().each())
        {
            if (!target.has_value())
            {
                target = {monster, std::sqrt(std::pow(x - monsterPos.x, 2) + std::pow(y - monsterPos.y, 2))};
                continue;
            }
            const auto dist = std::sqrt(std::pow(x - monsterPos.x, 2) + std::pow(y - monsterPos.y, 2));
            if (dist < target->second)
                target = {monster, dist};
        }
        if (target.has_value())
        {
            _registry.addComponent(player, Target{target->first});
        }
    }

    void updateAnimation()
    {
        if (!_registry.has_any_of<Animation>(player)) {
            auto& sp = _registry.get<Sprite>(player);
            sp.textureRect = sf::IntRect{0, 0, 192, 192};
            sp.texture = "assets/orchera/Factions/Knights/Troops/Archer/Red/Archer_Red.png";
            _registry.addComponent(player, Animation{
               .elapsedTime = 0,
               .frameSize = {192, 192},
               .frameDuration = .20f,
               .frameCount = 6,
               .loop = true,
               .currentFrame = 0,
               .onEnd = [](Entity) {}
            });
        }
    }
};

#endif //PLAYER_UPDATE_SYSTEM_HPP
