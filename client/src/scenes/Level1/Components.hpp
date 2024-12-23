/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Components.hpp
*/

#ifndef LEVEL1_COMPONENTS_HPP
#define LEVEL1_COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <cstdint>

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
    double range;
    std::uint32_t id;
};

struct Parallax {};

struct Animation {
    sf::Clock clock = sf::Clock();
    std::pair<int, int> frameSize{};
    float speed{};
    unsigned int currentFrame = 1;
    unsigned int frameCount{};
    bool loop{};
    Velocity velocity{};
};

struct Enemy {
    std::uint32_t id;
};

struct Bug {
    sf::Clock clock = sf::Clock();
};

struct Ship
{
    std::uint32_t id;
};

#endif //LEVEL1_COMPONENTS_HPP
