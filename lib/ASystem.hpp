/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ASystem.hpp
*/

#ifndef ASYSTEM_HPP
#define ASYSTEM_HPP

#include "Registry.hpp"

class ASystem {
public:
    ASystem(Registry& registry) : _registry(registry) {}
    virtual~ASystem() = default;

    /**
     * @brief Execute the system's logic
     */
    virtual void execute() = 0;

protected:
    Registry& _registry;
};

#endif //ASYSTEM_HPP

