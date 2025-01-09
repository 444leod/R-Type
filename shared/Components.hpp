/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Components.hpp
*/

#ifndef SHARED_COMPONENTS_HPP
#define SHARED_COMPONENTS_HPP

#include <cstdint>
#include <string>

struct Client
{
    ntw::ClientInformation info;

    explicit Client(const ntw::ClientInformation& info) : info(info) {}
    Client(const Client& other) = default;
};

#endif //SHARED_COMPONENTS_HPP
