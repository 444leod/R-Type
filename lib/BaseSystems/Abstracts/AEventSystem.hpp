/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AEventSystem.hpp
*/

#ifndef A_EVENT_SYSTEM_HPP
#define A_EVENT_SYSTEM_HPP

#include "ASystem.hpp"
#include "ecs/Registry.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class AEventSystem : public ASystem
{
public:
    AEventSystem(ecs::Registry &registry, const std::string& name) : ASystem(registry, name) {}

    /**
     * @brief Execute a 'render' system's logic
     * @param window The window to render to
     * @param resourcesManager The resources manager
     */
    virtual void execute(sf::Event& event) = 0;
};

#endif //A_EVENT_SYSTEM_HPP
