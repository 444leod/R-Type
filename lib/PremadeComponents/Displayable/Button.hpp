/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Button.hpp
*/

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "PremadeComponents/Displayable/Sprite.hpp"
#include "PremadeComponents/Displayable/Text.hpp"
#include "PremadeComponents/Shapes/Rectangle.hpp"

#include <functional>
#include <variant>

struct Button {
    std::variant<Sprite, shape::Rectangle> shape;
    std::function<void()> onClick;
    std::optional<Text> label = std::nullopt;
};

#endif // BUTTON_HPP
