/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawTextsSystem
*/

#ifndef DRAWTEXTSSYSTEM_HPP_
#define DRAWTEXTSSYSTEM_HPP_

#include <map>
#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include "BaseComponents.hpp"

#include <SFML/Graphics.hpp>

class DrawTextsSystem final : public ARenderSystem
{
public:
    explicit DrawTextsSystem(ecs::Registry &registry) : ARenderSystem(registry, "DrawTextsSystem") {}

    void execute(sf::RenderWindow &window) override
    {
        this->_registry.view<Position, Text>().each([&](const auto &entity, auto &pos, auto &text) {
            if (!this->_fonts.contains(text.font)) {
                this->_fonts[text.font] = sf::Font();
                this->_fonts[text.font].loadFromFile(text.font);
            }
            this->_text.setString(text.message);
            this->_text.setFont(this->_fonts[text.font]);
            this->_text.setPosition(pos.x, pos.y);
            this->_text.setCharacterSize(text.fontSize);
            this->_text.setFillColor(sf::Color(text.color.r, text.color.g, text.color.b));
            window.draw(this->_text);
        });
    }

private:
    sf::Text _text;
    std::map<std::string, sf::Font> _fonts = {};
};

#endif /* !DRAWTEXTSSYSTEM_HPP_ */
