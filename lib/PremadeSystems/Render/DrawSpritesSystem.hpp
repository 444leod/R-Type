/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DrawSpritesSystem
*/

#ifndef DRAW_SPRITES_SYSTEM_HPP_
#define DRAW_SPRITES_SYSTEM_HPP_

#include "ECS/Entity.hpp"

#include "PremadeModules/Rendering/ResourcesManager.hpp"
#include "PremadeSystems/Abstracts/ARenderSystem.hpp"

#include "PremadeComponents/Displayable/Sprite.hpp"
#include "PremadeComponents/Transform.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>

class DrawSpritesSystem final : public ARenderSystem
{
  public:
    explicit DrawSpritesSystem(ResourcesManager& resourceManager) : ARenderSystem("DrawSpritesSystem"), _textures(resourceManager.textures()) {}

    void execute(sf::RenderWindow& window) override
    {
        sf::Sprite sp{};
        auto vec = std::vector<std::tuple<ecs::Entity, Sprite, Transform>>{};

        auto view = _registry.view<Sprite, Transform>();
        vec.reserve(view.size());

        view.each(
            [&](const ecs::Entity& entity, Sprite& sprite, const Transform& transform)
            {
                try {
                    if (!_textures.contains(sprite.texture))
                    {
                        sf::Texture texture;
                        if (!texture.loadFromFile(sprite.texture)) {
                            std::cerr << "Failed to load texture: " << sprite.texture << std::endl;
                            return;  // Skip this entity if texture loading fails
                        }
                        _textures[sprite.texture] = texture;
                    }

                    const auto& tex = _textures.at(sprite.texture);
                    if (!sprite.textureRect.has_value()) {
                        sprite.textureRect = IntRect(0, 0, tex.getSize().x, tex.getSize().y);
                    }
                    vec.emplace_back(entity, sprite, transform);
                } catch (const std::exception& e) {
                    std::cerr << "Error processing sprite for entity " << entity << ": " << e.what() << std::endl;
                }
            });

        std::ranges::sort(vec, [](const auto& a, const auto& b) { return std::get<2>(a).z < std::get<2>(b).z; });

        for (const auto& [entity, sprite, transform] : vec)
        {
            try {
                if (!_textures.contains(sprite.texture)) continue;
                
                sp.setTexture(_textures.at(sprite.texture));
                sp.setScale(sprite.scale.first, sprite.scale.second);
                sp.setOrigin(sprite.origin.first, sprite.origin.second);
                sp.setPosition(transform.x, transform.y);
                sp.setRotation(transform.rotation);
                if (sprite.textureRect.has_value())
                {
                    sp.setTextureRect(sf::IntRect(
                        sprite.textureRect->left,
                        sprite.textureRect->top,
                        sprite.textureRect->width,
                        sprite.textureRect->height
                    ));
                }
                window.draw(sp);
            } catch (const std::exception& e) {
                std::cerr << "Error drawing sprite for entity " << entity << ": " << e.what() << std::endl;
            }
        }
    }

  private:
    std::map<std::string, sf::Texture>& _textures;
};

#endif /* !DRAW_SPRITES_SYSTEM_HPP_ */
