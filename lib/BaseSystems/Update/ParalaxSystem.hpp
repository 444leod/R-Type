/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ParalaxSystem
*/

#ifndef PARALLAXSYSTEM_HPP_
#define PARALLAXSYSTEM_HPP_

#include "BaseComponents.hpp"
#include "BaseSystems/Abstracts/AUpdateSystem.hpp"

class ParallaxSystem final : public AUpdateSystem {
  public:
    ParallaxSystem(Registry& registry, const std::string name = "ParallaxSystem") : AUpdateSystem(registry, name) {}

    void execute(const double deltaTime) override {
        _registry.view<Parallax, Transform>().each([&](Parallax& parallax, Transform& transform) {
            parallax.offset += static_cast<float>(deltaTime * parallax.offsetMultiplier);
            transform.x = -parallax.offset;
        });
    }
};

#endif /* !PARALLAXSYSTEM_HPP_ */
