/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawTextsSystem
*/

#ifndef DRAWTEXTSSYSTEM_HPP_
#define DRAWTEXTSSYSTEM_HPP_

#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include "BaseComponents.hpp"

#include <SFML/Graphics.hpp>

class DrawTextsSystem final : public ARenderSystem
{
public:
    DrawTextsSystem(Registry &registry) : ARenderSystem(registry) {}

    void execute(sf::RenderWindow &window) override {
        _registry.view<Position, Text>().each([&](const auto& entity, auto& pos, auto& renderable) {
            renderable.text.setPosition(pos.x, pos.y);
            window.draw(renderable.text);
        });
    }

private:
    const std::string _name = "DrawTextsSystem";
};

#endif /* !DRAWTEXTSSYSTEM_HPP_ */
