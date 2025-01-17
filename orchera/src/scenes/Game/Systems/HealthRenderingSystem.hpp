/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** HealthRenderingSystem.hpp
*/

#ifndef HEALTH_RENDERING_SYSTEM_HPP
#define HEALTH_RENDERING_SYSTEM_HPP

#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include "BaseComponents.hpp"

#include "../../../Components/Health.hpp"

class HealthRenderingSystem final : public ARenderSystem
{
public:
    explicit HealthRenderingSystem(ecs::Registry &registry) : ARenderSystem(registry, "HealthRenderingSystem")
    {
    }

    void execute(sf::RenderWindow &window) override
    {
        _registry.view<Transform, Health>().each([&](const Entity& entity, const Transform& transform, const Health& health) {
            if (health.health <= 0)
                return;
            constexpr float barWidth = 40.0f;
            constexpr float barHeight = 4.0f;
            sf::RectangleShape shape(sf::Vector2f(barWidth, barHeight));
            shape.setPosition(transform.x, transform.y - 20);
            shape.setFillColor(sf::Color::Red);
            shape.setOrigin(barWidth / 2, barHeight / 2);
            window.draw(shape);

            const float healthPercentage = static_cast<float>(health.health) / static_cast<float>(health.maxHealth);

            shape.setSize(sf::Vector2f(barWidth * healthPercentage, barHeight));
            shape.setFillColor(sf::Color::Green);
            window.draw(shape);
        });
    }

private:
};

#endif //HEALTH_RENDERING_SYSTEM_HPP
