/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BaseComposants
*/

#ifndef BASECOMPONENTS_HPP_
#define BASECOMPONENTS_HPP_

#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <functional>

// TODO: Decide how to organize the components in the lib

struct Transform {
    float x, y, z;
    float rotation;
};

struct Velocity {
    float x, y;
};

struct Self {};

struct Hitbox {};

struct Projectile {
    std::uint32_t id;
};

struct Parallax {
    int offsetMultiplier;
    float offset = 0;
};

struct Position {
    float x, y;
};

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
    std::function<void(Entity entity)> onEnd = [](Entity entity) {};
};

struct Enemy {
    std::uint32_t id;
};

// UI Components

struct Text {
    sf::Text text;
};

struct RectangleButton {
    sf::RectangleShape shape;
    std::string label;
    std::function<void()> onClick;
};

#endif /* !BASECOMPONENTS_HPP_ */
