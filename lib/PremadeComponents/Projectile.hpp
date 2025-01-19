/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Projectile.hpp
*/

#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <cstdint>

struct Projectile
{
    bool piercing;
    double range;
    std::uint8_t pierce;
    std::int32_t damage;
    std::uint32_t id;
};

#endif // PROJECTILE_HPP
