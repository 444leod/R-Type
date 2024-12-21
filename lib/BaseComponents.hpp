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

struct Text {
    sf::Text text;
};

struct RectangleButton {
    sf::RectangleShape shape;
    std::string label;
    std::function<void()> onClick;
};

#endif /* !BASECOMPONENTS_HPP_ */
