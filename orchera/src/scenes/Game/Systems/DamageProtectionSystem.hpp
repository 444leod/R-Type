/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DamageProtectionSystem.hpp
*/

#ifndef DAMAGE_PROTECTION_SYSTEM_HPP
#define DAMAGE_PROTECTION_SYSTEM_HPP

#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseComponents.hpp"

#include "../../../Components/DamageProtection.hpp"

class DamageProtectionSystem final : public AUpdateSystem
{
public:
    explicit DamageProtectionSystem(ecs::Registry &registry) : AUpdateSystem(registry, "DamageProtectionSystem")
    {
    }

    void execute(const double& deltaTime)
    {
        _registry.view<DamageProtection>().each([&](const Entity& entity, DamageProtection& damageProtection) {
            damageProtection.duration -= deltaTime;
            if (damageProtection.duration <= 0)
                _registry.removeComponent<DamageProtection>(entity);
        });
    }

private:
};

#endif //DAMAGE_PROTECTION_SYSTEM_HPP
