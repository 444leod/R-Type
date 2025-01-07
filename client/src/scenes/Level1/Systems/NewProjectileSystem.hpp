/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NewProjectileSystem
*/

#ifndef NEWPROJECTILESYSTEM_HPP_
#define NEWPROJECTILESYSTEM_HPP_

#include "BaseSystems/Abstracts/ASystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"
#include <config.h>
#include "../Events/PacketInformations.hpp"
#include <SFML/Graphics.hpp>

class NewProjectileSystem final : public ASystem
{
public:
    NewProjectileSystem(Registry &registry) : ASystem(registry, "NewProjectileSystem") {}

    void execute(const PacketInformations &event, const sf::Texture &projectileTex) {
        std::uint32_t projectileId; 
        std::uint32_t shipId;
        event.packet >> shipId >> projectileId;
        for (auto [shipEntity, ship, transform] : _registry.view<Ship, Transform>().each())
        {
            if (ship.id != shipId)
                continue;
            const auto projectile = _registry.create();
            const auto shootTransform = Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0};
            auto projectileSprite = sf::Sprite(projectileTex);
            projectileSprite.setOrigin(0, 0);
            projectileSprite.setScale(SCALE, SCALE);
            projectileSprite.setPosition(shootTransform.x, shootTransform.y);
            projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
            _registry.addComponent(projectile, Hitbox{});
            _registry.addComponent(projectile, projectileSprite);
            _registry.addComponent(projectile, shootTransform);
            _registry.addComponent(projectile, Projectile{ .id = projectileId });
            _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .speed = 20, .frameCount = 3, .loop = false, .onEnd = [&](Entity entity){
                _registry.removeComponent<Animation>(entity);
                _registry.addComponent(entity, Velocity{.x = 200, .y = 0});
            }});
        }
    }
};


#endif /* !NEWPROJECTILESYSTEM_HPP_ */
