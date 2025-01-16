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
    float attackSpeed;
    float range;
    std::uint8_t pierce;
    std::int32_t damage;
};

#endif //BOW_HPP
