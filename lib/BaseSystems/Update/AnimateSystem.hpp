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
    AnimateSystem(Registry &registry) : AUpdateSystem(registry) {}

    void execute(const double deltaTime, const sf::RenderWindow &window) override {
        _registry.view<Animation, sf::Sprite>().each([&](const Entity& entity, Animation & animation, sf::Sprite &sprite) {
            if (animation.currentFrame == animation.frameCount) {
                if (animation.loop) {
                    animation.currentFrame = 0;
                } else {
                    if (animation.velocity.x != 0 || animation.velocity.y != 0)
                        _registry.addComponent(entity, animation.velocity);
                    else
                        _registry.remove(entity);
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

private:
    const std::string _name = "AnimateSystem";
};

#endif /* !ANIMATESYSTEM_HPP_ */
