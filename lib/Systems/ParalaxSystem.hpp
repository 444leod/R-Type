/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ParalaxSystem
*/

#ifndef PARALLAXSYSTEM_HPP_
#define PARALLAXSYSTEM_HPP_

#include "Abstracts/AUpdateSystem.hpp"
#include "../BaseComponents.hpp"

class ParallaxSystem final : public AUpdateSystem
{
public:
    /** 
     * @brief Construct a new ParallaxSystem object
     * @param registry The registry to use
     * @param parallaxOffsetMultiplier The parallax offset multiplier, 
     */
    ParallaxSystem(Registry &registry, const int parallaxOffsetMultiplier = 1) : AUpdateSystem(registry), _parallaxOffsetMultiplier(parallaxOffsetMultiplier) {}

    void execute(const double deltaTime, const sf::RenderWindow &window) override {
        _registry.view<Parallax, Transform>().each([&](Parallax& parallax, Transform& transform) {
            parallax.offset += static_cast<float>(deltaTime * parallax.offsetMultiplier);
            transform.x = -parallax.offset;
        });
    }

private:
    const std::string _name = "ParallaxSystem";
    const int _parallaxOffsetMultiplier;
    float _parallaxOffset = 0;
};

#endif /* !PARALLAXSYSTEM_HPP_ */
