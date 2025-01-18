/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ButtonClickedSystemSystem.hpp
*/

#ifndef BUTTON_CLICKED_SYSTEM_HPP
#define BUTTON_CLICKED_SYSTEM_HPP

#include "PremadeSystems/Abstracts/AEventSystem.hpp"

#include "PremadeComponents/Displayable/Button.hpp"
#include "PremadeComponents/Transform.hpp"

class ButtonClickedSystem final : public AEventSystem
{
  public:
    explicit ButtonClickedSystem() : AEventSystem("ButtonClickedSystem") {}

    void _execution(sf::Event& event) override
    {
        if (event.type != sf::Event::MouseButtonPressed)
            return;
        if (event.mouseButton.button != sf::Mouse::Left)
            return;

        _registry.view<Transform, Button>().each(
            [&](const Transform& transform, const Button& button)
            {
                auto& [shape, onClick, text] = button;
                auto& [x, y, z, rotation] = transform;

                if (std::holds_alternative<shape::Rectangle>(shape))
                {
                    auto [width, height, fillColor, outlineColor, outlineThickness] = std::get<shape::Rectangle>(shape);

                    if (event.mouseButton.x < x || event.mouseButton.x > x + width)
                        return;

                    if (event.mouseButton.y < y || event.mouseButton.y > y + height)
                        return;
                }
                else
                {
                    const auto sp = std::get<Sprite>(shape);

                    if (!sp.textureRect.has_value())
                        return;

                    auto texRect = sp.textureRect.value();
                    texRect.width *= sp.scale.first;
                    texRect.height *= sp.scale.second;

                    if (event.mouseButton.x < x || event.mouseButton.x > x + texRect.width)
                        return;
                    if (event.mouseButton.y < y || event.mouseButton.y > y + texRect.height)
                        return;
                }

                onClick();
            });
    }
};

#endif // BUTTON_CLICKED_SYSTEM_HPP
