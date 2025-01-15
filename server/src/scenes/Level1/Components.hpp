/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Components.hpp
*/

#ifndef LEVEL1_COMPONENTS_HPP
#define LEVEL1_COMPONENTS_HPP

#include "BaseComponents.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>

struct Bug {
    sf::Clock clock = sf::Clock();
};

struct Ship
{
    std::uint32_t id;
};

#endif // LEVEL1_COMPONENTS_HPP
