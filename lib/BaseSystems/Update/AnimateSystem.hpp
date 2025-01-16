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
#include "Config.hpp"


class AnimateSystem final : public AUpdateSystem
{
public:
    explicit AnimateSystem(ecs::Registry &registry) : AUpdateSystem(registry, "AnimateSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Animation, Sprite>().each([&](const Entity& entity, Animation & animation, Sprite &sprite) {
            if (animation.currentFrame == animation.frameCount) {
                if (animation.loop) {
                    animation.currentFrame = 0;
                } else {
                    const std::function<void(Entity entity)> func = animation.onEnd;
                    // try {
                    //     _registry.get<Animation>(entity);
                    // }
                    // catch (std::out_of_range&) {
                    //     return;
                    // }
                    _registry.removeComponent<Animation>(entity);
                    func(entity);
                    return;
                }
            }
            if (animation.elapsedTime >= animation.frameDuration) {
                sprite.textureRect = sf::IntRect(animation.currentFrame * animation.frameSize.first, sprite.textureRect.value().top, animation.frameSize.first, animation.frameSize.second);
                animation.currentFrame++;
                animation.elapsedTime = 0;
            }
            animation.elapsedTime += deltaTime;
        });
    }
};

#endif /* !ANIMATESYSTEM_HPP_ */
