/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Game.hpp
*/

#ifndef LEVEL1_SPRITES_HPP
#define LEVEL1_SPRITES_HPP

#include "PremadeComponents/Displayable/Sprite.hpp"
#include "Config.hpp"

inline const auto spaceshipSprite = Sprite{"assets/r-typesheet42.gif", {SCALE, SCALE}, {0, 0}, IntRect(0, 0, 0, 0)};

inline const auto projectileSprite = Sprite{"assets/r-typesheet1.gif", {SCALE, SCALE}, {0, 0}, IntRect(0, 0, 0, 0)};

inline const auto backgroundSprite = Sprite{"assets/rtype-background.png", {SCALE, SCALE}, {0, 0}, IntRect(0, 243, 3072, 205)};

inline const auto bugSprite = Sprite{"assets/r-typesheet8.gif", {SCALE, SCALE}, {16, 13}};

inline const auto explosionSprite = Sprite{"assets/r-typesheet44.gif", {SCALE, SCALE}, {16, 16}, IntRect(131, 0, 192, 32)};

#endif // LEVEL1_SPRITES_HPP
