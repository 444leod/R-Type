/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Bow.hpp
*/

#ifndef BOW_HPP
#define BOW_HPP

#include "ecs/Registry.hpp"

#include <functional>

struct Bow
{
    float shootDelay;
    float shootCooldown;
    float range;
    std::function<void(ecs::Registry&, const Entity&, const std::pair<float, float>&, Bow&)> shoot;
};

#endif //BOW_HPP
