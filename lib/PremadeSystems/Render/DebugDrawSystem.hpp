/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DebugDrawSystem
*/

#ifndef DEBUG_DRAW_SYSTEM_HPP_
#define DEBUG_DRAW_SYSTEM_HPP_

#include "ECS/Entity.hpp"

#include "PremadeSystems/Abstracts/ARenderSystem.hpp"

#include "PremadeComponents/Tags/Debug.hpp"
#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Transform.hpp"

class DebugDrawSystem final : public ARenderSystem
{
public:
    DebugDrawSystem() : ARenderSystem("DebugDrawSystem") {}

    void execute(sf::RenderWindow &window) override {

        auto view = _registry.view<Debug, Hitbox, Transform>();

        view.each([&] (const ecs::Entity& entity, const Debug&, const Hitbox& hitbox, const Transform& transform) {
            if (std::holds_alternative<shape::Rectangle>(hitbox.shape)) {
                const auto&[width, height, fillColor, outlineColor, outlineThickness] = std::get<shape::Rectangle>(hitbox.shape);
                sf::RectangleShape shape(sf::Vector2f(width, height));
                shape.setPosition(transform.x + width / 2, transform.y + height / 2);
                shape.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillColor.a));
                shape.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
                shape.setOutlineThickness(outlineThickness);
                shape.setRotation(transform.rotation);
                shape.setOrigin(width / 2, height / 2);
                window.draw(shape);
            } else if (std::holds_alternative<shape::Circle>(hitbox.shape)) {
                const auto&[radius, fillColor, outlineColor, outlineThickness] = std::get<shape::Circle>(hitbox.shape);
                sf::CircleShape shape(radius);
                shape.setPosition(transform.x, transform.y);
                shape.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillColor.a));
                shape.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
                shape.setOutlineThickness(outlineThickness);
                shape.setOrigin(radius, radius);
                window.draw(shape);
            }
        });

    }
private:
};

#endif /* !DEBUG_DRAW_SYSTEM_HPP_ */
