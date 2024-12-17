/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Components.hpp
*/

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>

struct Transform {
    float x, y, z;
    float rotation;
};

struct Velocity {
    float x, y;
};

struct Self {};

struct Hitbox {};

struct Projectile {};

struct Parallax {};

struct Animation {
    sf::Clock clock = sf::Clock();
    std::pair<int, int> frameSize{};
    std::pair<int, int> frameOrigin{};
    float speed{};
    unsigned int currentFrame = 1;
    unsigned int frameCount{};
    bool loop{};
    Velocity velocity{};
};

struct Enemy {};

struct Bug {
    sf::Clock clock = sf::Clock();
};

struct Fast {
    sf::Clock clock = sf::Clock();
};

struct Debug {};

#endif //COMPONENTS_HPP
