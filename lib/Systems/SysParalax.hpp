/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Paralax
*/

#ifndef SYSPARALAX_HPP_
#define SYSPARALAX_HPP_

#include "../ASystem.hpp"
#include "../BaseComponents.hpp"

class SysParalax final: public ASystem {
public:
    SysParalax(Registry &registry, const int parallaxOffsetMultiplier) : ASystem(registry), _parallaxOffsetMultiplier(parallaxOffsetMultiplier) {}

    void execute(const double deltaTime) {
        _parallaxOffset += static_cast<float>(deltaTime * _parallaxOffsetMultiplier);

        _registry.view<Parallax, Transform>().each([&](const Parallax&, Transform& transform) {
        transform.x = -_parallaxOffset;
    });
    }

private:
    const int _parallaxOffsetMultiplier = 1;
    float _parallaxOffset = 0;
};

#endif /* !SYSPARALAX_HPP_ */
