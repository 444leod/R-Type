/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** OnLeftClickSystem
*/

#ifndef ONLEFTCLICKSYSTEM_HPP_
#define ONLEFTCLICKSYSTEM_HPP_

#include "BaseSystems/Abstracts/AOnEventSystem.hpp"
#include "BaseComponents.hpp"
#include <SFML/Graphics.hpp>

class OnLeftClickSystem final : public AOnEventSystem
{
public:
    OnLeftClickSystem(Registry &registry) : AOnEventSystem(registry) {}

    void execute(sf::Event &event) override {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            _registry.view<Position, RectangleButton>().each([&](const auto& entity, auto& pos, auto& button) {
                if (button.shape.getGlobalBounds().contains(mousePos)) {
                    button.onClick();
                    return;
                }
            });
        }
    }

private:
    const std::string _name = "OnLeftClickSystem";
};

#endif /* !ONLEFTCLICKSYSTEM_HPP_ */
