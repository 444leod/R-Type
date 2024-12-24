/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ParalaxSystem
*/

#ifndef PARALLAXSYSTEM_HPP_
#define PARALLAXSYSTEM_HPP_

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"

class ParallaxSystem final : public AUpdateSystem
{
public:
    ParallaxSystem(ecs::Registry &registry) : AUpdateSystem(registry, "ParallaxSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Parallax, Transform>().each([&](Parallax& parallax, Transform& transform) {
            parallax.offset += static_cast<float>(deltaTime * parallax.offsetMultiplier);
            transform.x = -parallax.offset;
        });
    }
};

#endif /* !PARALLAXSYSTEM_HPP_ */
