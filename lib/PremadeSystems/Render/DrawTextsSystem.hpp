/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawTextsSystem
*/

#ifndef DRAW_TEXTS_SYSTEM_HPP_
#define DRAW_TEXTS_SYSTEM_HPP_

#include "PremadeSystems/Abstracts/ARenderSystem.hpp"

#include "PremadeModules/Rendering/ResourcesManager.hpp"

#include "PremadeComponents/Displayable/Text.hpp"
#include "PremadeComponents/Transform.hpp"

#include <SFML/Graphics.hpp>
#include <map>

class DrawTextsSystem final : public ARenderSystem
{
  public:
    explicit DrawTextsSystem(ResourcesManager& resourceManager) : ARenderSystem("DrawTextsSystem"), _fonts(resourceManager.fonts()) {}

    void execute(sf::RenderWindow& window) override
    {
        this->_registry.view<Transform, Text>().each(
            [&](const auto&, auto& transform, auto& text)
            {
                if (!this->_fonts.contains(text.font))
                {
                    auto font = sf::Font{};
                    if (!font.loadFromFile(text.font))
                        std::cerr << "Failed to load font: " << text.font << std::endl;
                    this->_fonts[text.font] = font;
                    return;
                }
                this->_text.setString(text.message);
                this->_text.setFont(this->_fonts.at(text.font));
                this->_text.setPosition(transform.x, transform.y);
                this->_text.setCharacterSize(text.fontSize);
                this->_text.setFillColor(sf::Color(text.color.r, text.color.g, text.color.b, text.color.a));
                window.draw(this->_text);
            });
    }

  private:
    sf::Text _text;
    std::map<std::string, sf::Font>& _fonts;
};

#endif /* !DRAW_TEXTS_SYSTEM_HPP_ */
