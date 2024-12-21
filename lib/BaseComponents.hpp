/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BaseComposants
*/

#ifndef BASECOMPONENTS_HPP_
#define BASECOMPONENTS_HPP_

#include <cstdint>

struct Transform {
    float x, y, z;
    float rotation;
};

struct Velocity {
    float x, y;
};

struct Self {};

struct Hitbox {};

struct Projectile {
    std::uint32_t id;
};

struct Parallax {};

#endif /* !BASECOMPONENTS_HPP_ */
