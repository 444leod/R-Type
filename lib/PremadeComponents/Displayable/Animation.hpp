/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Animation.hpp
*/

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "ECS/Entity.hpp"

#include <cstdint>
#include <functional>

/**
 * @struct Animation
 * @brief Represents an animation component.
 *
 * This struct holds the properties and behaviors of an animation, including
 * frame size, duration, and looping behavior.
 */
struct Animation {
    /**
     * @brief The time elapsed since the last frame.
     */
    float elapsedTime = 0;

    /**
     * @brief The size of a frame (width, height).
     */
    std::pair<int, int> frameSize{};

    /**
     * @brief The duration of a frame in seconds.
     */
    float frameDuration{};

    /**
     * @brief The total number of frames in the animation.
     */
    unsigned int frameCount{};

    /**
     * @brief Indicates whether the animation should loop.
     */
    bool loop{};

    /**
     * @brief The index of the current frame (1-based).
     */
    unsigned int currentFrame = 1;

    /**
     * @brief The function to call when the animation ends (if loop is false).
     * @param entity The Entity that the animation is attached to.
     *
     * This function takes an Entity as a parameter and is called when the
     * animation completes.
     */
    std::function<void(const ecs::Entity& entity)> onEnd = [](const ecs::Entity&) {};
};

#endif // ANIMATION_HPP
