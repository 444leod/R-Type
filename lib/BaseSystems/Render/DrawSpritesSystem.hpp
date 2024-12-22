/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawSpritesSystem
*/

#ifndef DRAWSPRITESSYSTEM_HPP_
#define DRAWSPRITESSYSTEM_HPP_

#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include "BaseComponents.hpp"

#include <algorithm>
#include <SFML/Graphics.hpp>
#include <ranges>

class DrawSpritesSystem final : public ARenderSystem
{
public:
    DrawSpritesSystem(Registry &registry) : ARenderSystem(registry) {}

    void execute(sf::RenderWindow &window) override {
        auto vec = std::vector<std::tuple<Entity, sf::Sprite, Transform>>{};

        _registry.view<sf::Sprite, Transform>().each([&](const Entity& entity, sf::Sprite &sprite, Transform &transform) {
            vec.emplace_back(entity, sprite, transform);
            sprite.setPosition(transform.x, transform.y);
            sprite.setRotation(transform.rotation);
        });

        std::ranges::sort(vec, [](const auto& a, const auto& b) {
            return std::get<2>(a).z < std::get<2>(b).z;
        });

        for (const auto& [entity, sprite, _] : vec) {
            window.draw(sprite);
        }
    }

private:
    const std::string _name = "DrawSpritesSystem";
};

#endif /* !DRAWSPRITESSYSTEM_HPP_ */
