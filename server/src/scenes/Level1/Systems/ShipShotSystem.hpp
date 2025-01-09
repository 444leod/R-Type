/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipShotSystem
*/

#ifndef SHIP_SHOT_SYSTEM_HPP_
#define SHIP_SHOT_SYSTEM_HPP_


#include "BaseSystems/Abstracts/ASystem.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "../Events/PacketInformation.hpp"
#include <SFML/Graphics.hpp>

class ShipShotSystem final : public ASystem
{
public:
    ShipShotSystem(ecs::Registry &registry, RestrictedSceneManager &_manager) : ASystem(registry, "ShipShotSystem"), _manager(_manager) {}

    void execute(const PacketInformation &event, const sf::Texture &_projectileTex) const;
    
private:
    RestrictedSceneManager &_manager;
};

#endif /* !SHIP_SHOT_SYSTEM_HPP_ */
