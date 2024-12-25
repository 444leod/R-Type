/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipShootedSystem
*/

#ifndef SHIPSHOOTEDSYSTEM_HPP_
#define SHIPSHOOTEDSYSTEM_HPP_


#include "BaseSystems/Abstracts/ASystem.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "../Events/PacketInformations.hpp"
#include <SFML/Graphics.hpp>

class ShipShootedSystem final : public ASystem
{
public:
    ShipShootedSystem(ecs::Registry &registry, RestrictedSceneManager &_manager) : ASystem(registry, "ShipShootedSystem"), _manager(_manager) {}

    void execute(const PacketInformations &event, const sf::Texture &_projectileTex);
    
private:
    RestrictedSceneManager &_manager;
};

#endif /* !SHIPSHOOTEDSYSTEM_HPP_ */
