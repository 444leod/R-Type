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
     * @brief Execute the system's logic
     * @param deltaTime The time between the last frame and the current one
     * @param window The window to render to
     */
    virtual void execute(const double _deltaTime, const sf::RenderWindow &window) = 0;
};

#endif /* !AUPDATESYSTEM_HPP_ */
