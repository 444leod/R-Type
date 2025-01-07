/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AnimateSystem
*/

#ifndef ANIMATESYSTEM_HPP_
#define ANIMATESYSTEM_HPP_



#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"
#include <config.h>

class AnimateSystem final : public AUpdateSystem
{
public:
    AnimateSystem(Registry &registry, const std::string name = "AnimateSystem") : AUpdateSystem(registry, name) {}

    void execute(const double deltaTime) override {
        _registry.view<Animation, sf::Sprite>().each([&](const Entity& entity, Animation & animation, sf::Sprite &sprite) {
            if (animation.currentFrame == animation.frameCount) {
                if (animation.loop) {
                    animation.currentFrame = 0;
                } else {
                    animation.onEnd(entity);
                    _registry.removeComponent<Animation>(entity);
                }
            }
            if (animation.clock.getElapsedTime().asMilliseconds() >= animation.speed) {
                sprite.setTextureRect(sf::IntRect(animation.currentFrame * animation.frameSize.first, 0, animation.frameSize.first, animation.frameSize.second));
                animation.currentFrame++;
                animation.clock.restart();
            }
        });
    }
};

#endif /* !ANIMATESYSTEM_HPP_ */
