/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AEventSystem.hpp
*/

#ifndef A_EVENT_SYSTEM_HPP
#define A_EVENT_SYSTEM_HPP

#include "ASystem.hpp"
#include <SFML/Graphics.hpp>

class AEventSystem : public ASystem
{
  public:
    explicit AEventSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute a 'render' system's logic
     * @param window The window to render to
     * @param resourcesManager The resources manager
     */
    virtual void execute(sf::Event& event) = 0;
};

#endif // A_EVENT_SYSTEM_HPP
