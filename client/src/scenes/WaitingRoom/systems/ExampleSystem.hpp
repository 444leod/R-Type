/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ExampleRenderingSystem
*/

#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "BaseSystems/Abstracts/ARenderSystem.hpp"

class ExampleRenderingSystem final : public ARenderSystem
{
public:
    explicit ExampleRenderingSystem(ecs::Registry &registry) : ARenderSystem(registry, "ExampleRenderingSystem") {}

    void execute(sf::RenderWindow &window) override {
        sf::CircleShape circle(30.f, 32);
        circle.setOutlineThickness(2.f);
        circle.setOutlineColor(sf::Color::Red);
        circle.setOrigin(30.f, 30.f);
        circle.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
        window.draw(circle);
    }

private:
    ecs::SparseSet<sf::Texture> _textures;
};
