/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Hitbox.hpp
*/

#ifndef HITBOX_HPP
#define HITBOX_HPP

#include "ECS/Entity.hpp"
#include "Shapes/Circle.hpp"
#include "Shapes/Rectangle.hpp"

#include <functional>
#include <variant>

struct Hitbox
{
    std::variant<shape::Rectangle, shape::Circle> shape;
    std::function<void(const ecs::Entity& other)> onCollision;
};

#endif // HITBOX_HPP
