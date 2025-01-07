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
#include "Entity.hpp"

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
    float elapsedTime = 0;
    std::pair<int, int> frameSize{};
    float spf{};
    float frameDuration{};
    unsigned int frameCount{};
    bool loop{};
    unsigned int currentFrame = 1;
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
