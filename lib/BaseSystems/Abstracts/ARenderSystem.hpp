/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ARenderSystem
*/

#ifndef ARENDERSYSTEM_HPP_
#define ARENDERSYSTEM_HPP_

#include "ASystem.hpp"
#include <SFML/Graphics.hpp>

class ARenderSystem : public ASystem
{
public:
    ARenderSystem(Registry &registry) : ASystem(registry) {}

    /**
     * @brief Execute the system's logic
     * @param window The window to render to
     */
    virtual void execute(sf::RenderWindow &window) = 0;
};

#endif /* !ARENDERSYSTEM_HPP_ */
