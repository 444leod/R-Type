/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Rectangle.hpp
*/

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "PremadeComponents/Color.hpp"

namespace shape
{

struct Rectangle
{
    float width;
    float height;
    Color fillColor = {0, 0, 0};
    Color outlineColor = {0, 0, 0};
    float outlineThickness = 0;
};

} // namespace shape

#endif //RECTANGLE_HPP
