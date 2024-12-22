/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AOnEventSystem
*/

#ifndef AONEVENTSYSTEM_HPP_
#define AONEVENTSYSTEM_HPP_

#include "ASystem.hpp"
#include <SFML/Graphics.hpp>

class AOnEventSystem : public ASystem
{
public:
    AOnEventSystem(Registry &registry) : ASystem(registry) {}

    /**
     * @brief Execute a 'event' system's logic
     * @param event The sfml event to handle
     */
    virtual void execute(sf::Event &event) = 0;
};

#endif /* !AONEVENTSYSTEM_HPP_ */
