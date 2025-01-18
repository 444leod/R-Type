/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DamageProtectionSystem.hpp
*/

#ifndef DAMAGE_PROTECTION_SYSTEM_HPP
#define DAMAGE_PROTECTION_SYSTEM_HPP


#include <Engine/Systems/AUpdateSystem.hpp>

#include "../../../Components/DamageProtection.hpp"

#include "Config.hpp"


class DamageProtectionSystem final : public engine::AUpdateSystem
{
public:
    DamageProtectionSystem() : AUpdateSystem("DamageProtectionSystem")
    {
    }

    void execute(const double& deltaTime)
    {
        _registry.view<DamageProtection>().each([&](const ecs::Entity& entity, DamageProtection& damageProtection) {
            damageProtection.duration -= deltaTime;
            if (damageProtection.duration <= 0)
                _registry.removeComponent<DamageProtection>(entity);
        });
    }

private:
};

#endif //DAMAGE_PROTECTION_SYSTEM_HPP
