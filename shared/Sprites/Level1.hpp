/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Level1.hpp
*/

#ifndef LEVEL1_SPRITES_HPP
#define LEVEL1_SPRITES_HPP

#include "BaseComponents.hpp"

inline const auto spaceshipSprite = Sprite {
    "assets/r-typesheet42.gif",
    { 1, 1 },
    { 0, 0 },
    sf::IntRect(0, 0, 34, 18)
};

inline const auto projectileSprite = Sprite {
    "assets/r-typesheet1.gif",
    { 1, 1 },
    { 0, 0 },
    sf::IntRect(0, 91, 48, 16)
};

inline const auto backgroundSprite = Sprite {
    "assets/rtype-background.png",
    { 1, 1 },
    { 0, 0 },
    sf::IntRect(0, 243, 3072, 205)
};

inline const auto bugSprite = Sprite {
    "assets/r-typesheet8.gif",
    { 1, 1 },
    { 16, 13 }
};

inline const auto explosionSprite = Sprite {
    "assets/r-typesheet44.gif",
    { 1, 1 },
    { 16, 16 },
    sf::IntRect(131, 0, 192, 32)
};

#endif //LEVEL1_SPRITES_HPP
