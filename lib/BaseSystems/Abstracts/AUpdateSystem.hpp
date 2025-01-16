/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AUpdateSystem
*/

#ifndef AUPDATESYSTEM_HPP_
#define AUPDATESYSTEM_HPP_

#include "ASystem.hpp"
#include <string>

class AUpdateSystem : public ASystem
{
public:
    AUpdateSystem(ecs::Registry &registry, const std::string& name) : ASystem(registry, name) {}

    /**
     * @brief Execute a 'update' system's logic
     * @param deltaTime The time between the last frame and the current one
     */
    virtual void execute(const double& deltaTime) = 0;
};

#endif /* !AUPDATESYSTEM_HPP_ */
