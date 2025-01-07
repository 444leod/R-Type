/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewProjectileSystem
*/

#ifndef NEW_PROJECTILE_SYSTEM_HPP_
#define NEW_PROJECTILE_SYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"
#include "Config.hpp"
#include "../Events/PacketInformation.hpp"
#include <SFML/Graphics.hpp>

class NewProjectileSystem final : public ASystem
{
public:
    explicit NewProjectileSystem(ecs::Registry &registry) : ASystem(registry, "NewProjectileSystem") {}

    void execute(const std::uint32_t& shipId, const std::uint32_t& projectileId, const Transform& transform, const Velocity& velocity) {
        for (auto [entity, ship, transform] : _registry.view<Ship, Transform>().each())
        {
            if (ship.id != shipId)
                continue;
            const auto projectile = _registry.create();

            //TODO: update

            // auto projectileSprite = sf::Sprite(projectileTex);
            // projectileSprite.setOrigin(0, 0);
            // projectileSprite.setScale(SCALE, SCALE);
            // projectileSprite.setPosition(shootTransform.x, shootTransform.y);
            // projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
            // _registry.addComponent(projectile, projectileSprite);

            _registry.addComponent(projectile, Hitbox{});
            _registry.addComponent(projectile, transform);
            _registry.addComponent(projectile, Projectile{ .id = projectileId });
            _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .speed = 20, .frameCount = 3, .loop = false, .velocity = velocity});
            return;
        }
    }
};


#endif /* !NEWPROJECTILESYSTEM_HPP_ */
