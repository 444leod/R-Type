/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Color.hpp
*/

#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

struct Color
{
    std::int8_t r;
    std::int8_t g;
    std::int8_t b;
    std::int8_t a = static_cast<std::int8_t>(255);

    Color() = default;
    Color(std::int8_t r, std::int8_t g, std::int8_t b, std::int8_t a = 255) : r(r), g(g), b(b), a(a) {}
    Color(int r, int g, int b, int a = 255) : r(static_cast<std::int8_t>(r)), g(static_cast<std::int8_t>(g)), b(static_cast<std::int8_t>(b)), a(static_cast<std::int8_t>(a)) {}
};

#endif // COLOR_HPP
