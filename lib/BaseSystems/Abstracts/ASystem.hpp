/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ASystem.hpp
*/

#ifndef ASYSTEM_HPP
#define ASYSTEM_HPP

#include "Registry.hpp"
#include <string>

class ASystem {
public:
    ASystem(Registry& registry) : _registry(registry) {}
    virtual~ASystem() = default;

    /**
     * @brief Disables the system
     */
    void disable() { _enabled = false; }

    /**
     * @brief Enables the system
     */
    void enable() { _enabled = true; }

    /**
     * @brief Returns whether the system is enabled
     * @return Whether the system is enabled
     */
    bool isEnabled() const { return _enabled; }

    /**
     * @brief Returns the system's name
     * @return The system's name
     */
    std::string getName() const { return _name; }

protected:
    Registry& _registry;
    bool _enabled = true;
    std::string _name;
};

#endif //ASYSTEM_HPP

