/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Client.hpp
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Network/NetworkAgent.hpp"

struct Client
{
    ntw::ClientInformation info;

    explicit Client(const ntw::ClientInformation& info) : info(info) {}
    Client(const Client& other) = default;
};

#endif // CLIENT_HPP
