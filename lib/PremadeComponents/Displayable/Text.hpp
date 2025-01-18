/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Text.hpp
*/

#ifndef TEXT_HPP
#define TEXT_HPP

#include "PremadeComponents/Color.hpp"

#include <string>

struct Text {
  public:
    std::string font;
    std::string message;
    std::uint32_t fontSize;
    Color color;
};

#endif // TEXT_HPP
