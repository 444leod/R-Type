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
#include <string>

class ARenderSystem : public ASystem
{
public:
    ARenderSystem(Registry &registry, const std::string name) : ASystem(registry, name) {}

    /**
     * @brief Execute a 'render' system's logic
     * @param window The window to render to
     */
    virtual void execute(sf::RenderWindow &window) = 0;
};

#endif /* !ARENDERSYSTEM_HPP_ */
