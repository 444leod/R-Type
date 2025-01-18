/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Weapon.hpp
*/

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Bow.hpp"

#include <variant>

struct Weapon
{
    std::variant<Bow> weapon;
};

#endif //WEAPON_HPP
