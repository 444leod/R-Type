/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawSpritesSystem
*/

#ifndef DRAW_SPRITES_SYSTEM_HPP_
#define DRAW_SPRITES_SYSTEM_HPP_

#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include "BaseComponents.hpp"
#include "engine/modules/ResourcesManager.hpp"

#include <algorithm>
#include <SFML/Graphics.hpp>
#include <ranges>

class DrawSpritesSystem final : public ARenderSystem
{
public:
    explicit DrawSpritesSystem(ecs::Registry &registry, engine::ResourcesManager& resourceManager) : ARenderSystem(registry, "DrawSpritesSystem"), _textures(resourceManager.textures()) {}

    void execute(sf::RenderWindow &window) override
    {
        sf::Sprite sp{};
        auto vec = std::vector<std::tuple<Entity, Sprite, Transform>>{};

        auto view = _registry.view<Sprite, Transform>();
        vec.reserve(view.size());

        view.each([&](const Entity& entity, Sprite &sprite, const Transform &transform) {
            if (!_textures.contains(sprite.texture))
            {
                sf::Texture texture;
                if (!texture.loadFromFile(sprite.texture))
                    std::cerr << "Failed to load texture: " << sprite.texture << std::endl;
                _textures[sprite.texture] = texture;
            }

            const auto tex = _textures.at(sprite.texture);

            if (!sprite.textureRect.has_value())
            {
                sprite.textureRect = sf::IntRect(0, 0, tex.getSize().x, tex.getSize().y);
            }
            vec.emplace_back(entity, sprite, transform);
        });

        std::ranges::sort(vec, [](const auto& a, const auto& b) {
            return std::get<2>(a).z < std::get<2>(b).z;
        });

        for (const auto& [entity, sprite, transform] : vec) {
            sp.setTexture(_textures.at(sprite.texture));
            sp.setScale(sprite.scale.first, sprite.scale.second);
            sp.setOrigin(sprite.origin.first, sprite.origin.second);
            sp.setPosition(transform.x, transform.y);
            sp.setRotation(transform.rotation);
            if (sprite.textureRect.has_value()) {
                sp.setTextureRect(sprite.textureRect.value());
            }
            window.draw(sp);
        }
    }

private:
    std::map<std::string, sf::Texture>& _textures;
};

#endif /* !DRAW_SPRITES_SYSTEM_HPP_ */
