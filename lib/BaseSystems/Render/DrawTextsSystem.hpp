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
    DrawTextsSystem(Registry &registry, const std::string name = "DrawTextsSystem") : ARenderSystem(registry, name)
    {
        this->_font.loadFromFile("assets/arial.ttf");
        this->_text.setFont(this->_font);
    }

    void execute(sf::RenderWindow &window) override
    {
        this->_registry.view<Position, Text>().each([&](const auto &entity, auto &pos, auto &text) {
            std::cout << "Drawing following text:\n" << text.message << std::endl;
            this->_text.setString(text.message);
            this->_text.setPosition(pos.x, pos.y);
            this->_text.setCharacterSize(text.fontSize);
            this->_text.setFillColor(sf::Color(text.color.r, text.color.g, text.color.b));
            window.draw(this->_text);
        });
    }

private:
    sf::Text _text;
    sf::Font _font;
};

#endif /* !DRAWTEXTSSYSTEM_HPP_ */
