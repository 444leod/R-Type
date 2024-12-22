/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AUpdateSystem
*/

#ifndef AUPDATESYSTEM_HPP_
#define AUPDATESYSTEM_HPP_

#include "ASystem.hpp"
#include <SFML/Graphics.hpp>

class AUpdateSystem : public ASystem
{
public:
    AUpdateSystem(Registry &registry) : ASystem(registry) {}

    /**
     * @brief Execute a 'update' system's logic
     * @param deltaTime The time between the last frame and the current one
     * @param window The window used as a reference for the system for logic, not for rendering
     */
    virtual void execute(const double _deltaTime, const sf::RenderWindow &window) = 0;
};

#endif /* !AUPDATESYSTEM_HPP_ */
