/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BaseComposants
*/

#ifndef BASECOMPONENTS_HPP_
#define BASECOMPONENTS_HPP_

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

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

struct Animation {
    sf::Clock clock = sf::Clock();
    std::pair<int, int> frameSize{};
    float speed{};
    unsigned int currentFrame = 1;
    unsigned int frameCount{};
    bool loop{};
    // TODO: Refacto to not have nested components
    Velocity velocity{};
};

struct Enemy {
    std::uint32_t id;
};

// UI Components

struct Text {
    std::string message;
    std::uint32_t fontSize;
    struct {
        std::uint32_t r;
        std::uint32_t g;
        std::uint32_t b;
    } color;
};

struct RectangleButton {
    sf::RectangleShape shape;
    std::string label;
    std::function<void()> onClick;
};

#endif /* !BASECOMPONENTS_HPP_ */
