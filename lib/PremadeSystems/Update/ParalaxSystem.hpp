/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ParalaxSystem
*/

#ifndef PARALLAX_SYSTEM_HPP_
#define PARALLAX_SYSTEM_HPP_

#include <Engine/Systems/AUpdateSystem.hpp>

#include "SharedComponents/Parallax.hpp"
#include "PremadeComponents/Transform.hpp"

class ParallaxSystem final : public engine::AUpdateSystem
{
public:
    ParallaxSystem() : AUpdateSystem("ParallaxSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Parallax, Transform>().each([deltaTime](Parallax& parallax, Transform& transform) {
            parallax.offset += static_cast<float>(deltaTime * parallax.offsetMultiplier);
            transform.x = -parallax.offset;
        });
    }
};

#endif /* !PARALLAX_SYSTEM_HPP_ */
