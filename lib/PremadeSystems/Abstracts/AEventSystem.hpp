/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AEventSystem.hpp
*/

#ifndef A_EVENT_SYSTEM_HPP
#define A_EVENT_SYSTEM_HPP

#include <Engine/Systems/ASystem.hpp>
#include <SFML/Graphics.hpp>

class AEventSystem : public engine::ASystem
{
  public:
    explicit AEventSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute a 'event' system's logic
     *
     * @param event The event to handle
     */
    virtual void execute(sf::Event& event) = 0;
};

#endif // A_EVENT_SYSTEM_HPP
