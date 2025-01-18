/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawButtonsSystem
*/

#ifndef DRAW_BUTTONS_SYSTEM_HPP_
#define DRAW_BUTTONS_SYSTEM_HPP_

#include "ECS/Entity.hpp"

#include "PremadeModules/Rendering/ResourcesManager.hpp"
#include "PremadeSystems/Abstracts/ARenderSystem.hpp"

#include "PremadeComponents/Displayable/Button.hpp"
#include "PremadeComponents/Transform.hpp"

class DrawButtonsSystem final : public ARenderSystem
{
  public:
    explicit DrawButtonsSystem(ResourcesManager& resourcesManager) : ARenderSystem("DrawButtonsSystem"), _textures(resourcesManager.textures()), _fonts(resourcesManager.fonts()) {}

    void execute(sf::RenderWindow& window) override
    {
        sf::RectangleShape rectangle;
        sf::Sprite sprite;
        sf::Text label;
        _registry.view<Button, Transform>().each(
            [&](const ecs::Entity& entity, Button& button, const Transform& transform)
            {
                auto& [shape, _, text] = button;
                auto& [x, y, z, rotation] = transform;

                float shapeWidth, shapeHeight;

                if (std::holds_alternative<shape::Rectangle>(shape))
                {
                    auto [width, height, fillColor, outlineColor, outlineThickness] = std::get<shape::Rectangle>(button.shape);

                    rectangle.setSize({width, height});
                    rectangle.setPosition(x, y);
                    rectangle.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillColor.a));
                    rectangle.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
                    rectangle.setOutlineThickness(outlineThickness);

                    shapeWidth = width;
                    shapeHeight = height;

                    window.draw(rectangle);
                }
                else
                {
                    auto& sp = std::get<Sprite>(button.shape);
                    if (!_textures.contains(sp.texture))
                    {
                        sf::Texture texture;
                        if (!texture.loadFromFile(sp.texture))
                            std::cerr << "Failed to load texture: " << sp.texture << std::endl;
                        _textures[sp.texture] = texture;
                    }

                    const auto tex = _textures.at(sp.texture);

                    if (!sp.textureRect.has_value())
                    {
                        sp.textureRect = IntRect(0, 0, tex.getSize().x, tex.getSize().y);
                    }

                    sprite.setTexture(tex);
                    sprite.setTextureRect(sf::IntRect(sp.textureRect->top, sp.textureRect->left, sp.textureRect->width, sp.textureRect->height));
                    sprite.setScale(sp.scale.first, sp.scale.second);
                    sprite.setPosition(x, y);

                    shapeHeight = sprite.getGlobalBounds().height;
                    shapeWidth = sprite.getGlobalBounds().width;

                    window.draw(sprite);
                }

                if (!text.has_value())
                    return;

                if (!_fonts.contains(text->font))
                {
                    sf::Font font;
                    if (!font.loadFromFile(text->font))
                        std::cerr << "Failed to load font: " << text->font << std::endl;
                    _fonts[text->font] = font;
                }
                label.setFont(_fonts.at(text->font));
                label.setString(text->message);
                label.setCharacterSize(text->fontSize);
                label.setPosition(x + shapeWidth / 2 - label.getGlobalBounds().width / 2, y + shapeHeight / 2 / 2 - label.getGlobalBounds().height / 2);
                label.setFillColor(sf::Color(text->color.r, text->color.g, text->color.b));

                window.draw(label);
            });
    }

  private:
    std::map<std::string, sf::Texture> _textures;
    std::map<std::string, sf::Font> _fonts;
};

#endif /* !DRAW_BUTTONS_SYSTEM_HPP_ */
