//
// Created by nicolas on 2/8/25.
//

#ifndef DRAWINPUTSYSTEM_HPP
#define DRAWINPUTSYSTEM_HPP
#include "PremadeComponents/Displayable/Input.hpp"

#include <ECS/Entity.hpp>
#include <PremadeComponents/Transform.hpp>
#include <PremadeModules/Rendering/ResourcesManager.hpp>
#include <PremadeSystems/Abstracts/ARenderSystem.hpp>

class ResourcesManager;
class DrawInputsSystem final : public ARenderSystem
{
  public:
    explicit DrawInputsSystem(ResourcesManager& resourcesManager) : ARenderSystem("DrawButtonsSystem"), _fonts(resourcesManager.fonts()) {}

    void execute(sf::RenderWindow& window) override
    {
        sf::RectangleShape rectangle;
        sf::Text label;
        _registry.view<Input, Transform>().each(
            [&](const ecs::Entity& entity, Input& input, const Transform& transform)
            {
                if (input.active)
                    input.box.outlineColor = input.focused;
                else
                    input.box.outlineColor = input.notFocused;
                
                auto &text = input.text;
                auto& [x, y, z, rotation] = transform;

                auto [width, height, fillColor, outlineColor, outlineThickness] = input.box;

                rectangle.setSize({width, height});
                rectangle.setPosition(x, y);
                rectangle.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillColor.a));
                rectangle.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
                rectangle.setOutlineThickness(outlineThickness);

                const float shapeWidth = width;
                const float shapeHeight = height;

                window.draw(rectangle);

                if (input.value.empty())
                    text.message = input.placeHolder;
                else
                    text.message = input.value;
                
                if (!_fonts.contains(text.font))
                {
                    sf::Font font;
                    if (!font.loadFromFile(text.font))
                        std::cerr << "Failed to load font: " << text.font << std::endl;
                    _fonts[text.font] = font;
                }
                label.setFont(_fonts.at(text.font));
                label.setString(text.message);
                label.setCharacterSize(text.fontSize);
                label.setPosition(x + shapeWidth / 2 - label.getGlobalBounds().width / 2, y + shapeHeight / 2 / 2 - label.getGlobalBounds().height / 2);

                if (input.disabled)
                    label.setFillColor(sf::Color(100, 100, 100));
                else
                    label.setFillColor(sf::Color(text.color.r, text.color.g, text.color.b));

                if (label.getGlobalBounds().width > shapeWidth)
                {
                    label.setCharacterSize(text.fontSize * shapeWidth / label.getGlobalBounds().width);
                    label.setPosition(x + shapeWidth / 2 - label.getGlobalBounds().width / 2, y + shapeHeight / 2 / 2 - label.getGlobalBounds().height / 2);
                }

                window.draw(label);
            });
    }

  private:
    std::map<std::string, sf::Font> _fonts;
};

#endif //DRAWINPUTSYSTEM_HPP
