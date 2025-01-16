/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawShapeSystem
*/

#ifndef DRAW_SHAPE_SYSTEM_HPP_
#define DRAW_SHAPE_SYSTEM_HPP_

#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include "BaseComponents.hpp"

#include <algorithm>
#include <SFML/Graphics.hpp>
#include <ranges>

class DrawShapeSystem final : public ARenderSystem
{
public:
    explicit DrawShapeSystem(ecs::Registry &registry) : ARenderSystem(registry, "DrawShapeSystem") {}

    void execute(sf::RenderWindow &window) override {

        auto view = _registry.view<shape::Circle, Transform>();

        _registry.view<shape::Rectangle, Transform>().each(
            [&] (const Entity& entity, const shape::Rectangle& rect, const Transform& transform)
            {
                const auto&[width, height, fillColor, outlineColor, outlineThickness] = rect;
                sf::RectangleShape shape(sf::Vector2f(width, height));
                shape.setPosition(transform.x + width / 2, transform.y + height / 2);
                shape.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillColor.a));
                shape.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
                shape.setOutlineThickness(outlineThickness);
                shape.setRotation(transform.rotation);
                shape.setOrigin(width / 2, height / 2);
                window.draw(shape);
            }
        );

        _registry.view<shape::Circle, Transform>().each(
            [&] (const Entity& entity, const shape::Circle& circle, const Transform& transform)
            {
                const auto&[radius, fillColor, outlineColor, outlineThickness] = circle;
                sf::CircleShape shape(radius);
                shape.setPosition(transform.x, transform.y);
                shape.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillColor.a));
                shape.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
                shape.setOutlineThickness(outlineThickness);
                shape.setOrigin(radius / 2, radius / 2);
                window.draw(shape);
            }
        );

    }

private:
    ecs::SparseSet<sf::Texture> _textures;
};

#endif /* !DRAW_SHAPE_SYSTEM_HPP_ */
