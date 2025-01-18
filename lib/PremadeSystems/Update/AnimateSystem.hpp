/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AnimateSystem
*/

#ifndef ANIMATE_SYSTEM_HPP_
#define ANIMATE_SYSTEM_HPP_

#include "ECS/Entity.hpp"

#include <Engine/Systems/AUpdateSystem.hpp>

#include "PremadeComponents/Displayable/Animation.hpp"
#include "PremadeComponents/Displayable/Sprite.hpp"

class AnimateSystem final : public engine::AUpdateSystem
{
public:
    explicit AnimateSystem() : AUpdateSystem("AnimateSystem") {}

    void execute(const double& deltaTime) override {
        _registry.view<Animation, Sprite>().each([&](const ecs::Entity& entity, Animation & animation, Sprite &sprite) {
            if (animation.currentFrame == animation.frameCount) {
                if (animation.loop) {
                    animation.currentFrame = 0;
                } else {
                    const std::function<void(ecs::Entity entity)> func = animation.onEnd;
                    _registry.removeComponent<Animation>(entity);
                    func(entity);
                    return;
                }
            }
            if (animation.elapsedTime >= animation.frameDuration) {
                sprite.textureRect = IntRect(animation.currentFrame * animation.frameSize.first, sprite.textureRect.value().top, animation.frameSize.first, animation.frameSize.second);
                animation.currentFrame++;
                animation.elapsedTime = 0;
            }
            animation.elapsedTime += deltaTime;
        });
    }
};

#endif /* !ANIMATE_SYSTEM_HPP_ */
