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
#include "BaseComponents.hpp"

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
    uint32_t id;
};

struct Bug {
    sf::Clock clock = sf::Clock();
};

struct Ship
{
    std::uint32_t id;
};

#endif //LEVEL1_COMPONENTS_HPP
