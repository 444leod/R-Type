/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AnimateSystem
*/

#ifndef ANIMATESYSTEM_HPP_
#define ANIMATESYSTEM_HPP_

#include "BaseComponents.hpp"
#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include <config.h>

class AnimateSystem final : public AUpdateSystem {
  public:
    AnimateSystem(Registry& registry, const std::string name = "AnimateSystem") : AUpdateSystem(registry, name) {}

    void execute(const double deltaTime) override {
        _registry.view<Animation, sf::Sprite>().each([&](const Entity& entity, Animation& animation, sf::Sprite& sprite) {
            if (animation.currentFrame == animation.frameCount) {
                if (animation.loop) {
                    animation.currentFrame = 0;
                } else {
                    animation.onEnd(entity);
                    try {
                        _registry.get<Animation>(entity);
                    } catch (std::out_of_range) {
                        return;
                    }
                    _registry.removeComponent<Animation>(entity);
                    return;
                }
            }
            if (animation.elapsedTime >= animation.frameDuration) {
                sprite.setTextureRect(sf::IntRect(animation.currentFrame * animation.frameSize.first, 0, animation.frameSize.first, animation.frameSize.second));
                animation.currentFrame++;
                animation.elapsedTime = 0;
            }
            animation.elapsedTime += deltaTime;
        });
    }
};

#endif /* !ANIMATESYSTEM_HPP_ */
