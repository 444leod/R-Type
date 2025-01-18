/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Sprite.hpp
*/

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Utils/IntRect.hpp"

#include <string>
#include <optional>

struct Sprite
{
    std::string texture;
    std::pair<float, float> scale;
    std::pair<float, float> origin;
    std::optional<IntRect> textureRect = std::nullopt;
};

#endif //SPRITE_HPP
