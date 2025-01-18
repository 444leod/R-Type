/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Circle.hpp
*/

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "PremadeComponents/Color.hpp"

namespace shape
{
struct Circle
{
    float radius;
    Color fillColor;
    Color outlineColor;
    float outlineThickness;
};
} // namespace shape

#endif // CIRCLE_HPP
