/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BaseComposants
*/

#ifndef BASE_COMPONENTS_HPP_
#define BASE_COMPONENTS_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <filesystem>
#include <variant>
#include <SFML/Graphics.hpp>

#include "ecs/Entity.hpp"
#include "Config.hpp"

// TODO: Decide how to organize the components in the lib

struct Transform {
    float x, y, z;
    float rotation;
};

struct Velocity {
    float x, y;
};

struct Self {};

struct Color
{
    std::int8_t r;
    std::int8_t g;
    std::int8_t b;
    std::int8_t a = static_cast<std::int8_t>(255);

    Color() = default;
    Color(std::int8_t r, std::int8_t g, std::int8_t b, std::int8_t a = 255)
        : r(r), g(g), b(b), a(a)
    {
    }
    Color(int r, int g, int b, int a = 255)
        : r(static_cast<std::int8_t>(r)), g(static_cast<std::int8_t>(g)), b(static_cast<std::int8_t>(b)), a(static_cast<std::int8_t>(a))
    {
    }
};

struct Projectile {
    double range;
    std::uint32_t id;
};

struct Parallax {
    int offsetMultiplier = 0;
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
public:
    std::string font;
    std::string message;
    std::uint32_t fontSize;
    Color color;
};

class DrawSpritesSystem;

struct Sprite
{
public:
    std::string texture;
    std::pair<float, float> scale;
    std::pair<float, float> origin;
    std::optional<sf::IntRect> textureRect = std::nullopt;
};

namespace shape
{
struct Circle
{
    float radius;
    Color fillColor;
    Color outlineColor;
    float outlineThickness;
};

struct Rectangle
{
    float width;
    float height;
    Color fillColor = {0, 0, 0};
    Color outlineColor = {0, 0, 0};
    float outlineThickness = 0;
};

};

struct Hitbox
{
    std::variant<shape::Rectangle, shape::Circle> shape;
    std::function<void(const Entity& other)> onCollision;
};

#include <iostream>

struct Button {
    std::variant<Sprite, shape::Rectangle> shape;
    std::function<void()> onClick;
    std::optional<Text> label = std::nullopt;

};

struct Debug {};

#endif /* !BASECOMPONENTS_HPP_ */
