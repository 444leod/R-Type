/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawMenuButtonsSystem
*/

#ifndef DRAW_MENU_BUTTONS_SYSTEM_HPP_
#define DRAW_MENU_BUTTONS_SYSTEM_HPP_

#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include "BaseComponents.hpp"

#include <SFML/Graphics.hpp>

class DrawMenuButtonsSystem final : public ARenderSystem
{
public:
    explicit DrawMenuButtonsSystem(ecs::Registry &registry) : ARenderSystem(registry, "DrawMenuButtonsSystem") {}

    void execute(sf::RenderWindow& window) override {
        //TODO: Change this questionnable system
        sf::Font font;
        font.loadFromFile("assets/arial.ttf");
        _registry.view<Position, RectangleButton>().each([&](const auto& entity, auto& pos, auto& button) {
            button.shape.setPosition(pos.x, pos.y);
            window.draw(button.shape);

            sf::Text buttonText(button.label, font, 20);
            buttonText.setPosition(pos.x + 10, pos.y + 10);
            buttonText.setFillColor(sf::Color::White);
            window.draw(buttonText);
        });
    }
};

#endif /* !DRAW_MENU_BUTTONS_SYSTEM_HPP_ */
