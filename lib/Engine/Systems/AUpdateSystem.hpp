/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AUpdateSystem
*/

#ifndef A_UPDATE_SYSTEM_HPP_
#define A_UPDATE_SYSTEM_HPP_

#include <Engine/Systems/ASystem.hpp>

namespace engine
{

class AUpdateSystem : public engine::ASystem
{
  public:
    explicit AUpdateSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute a 'update' system's logic
     * @param deltaTime The time between the last frame and the current one
     */
    virtual void execute(const double& deltaTime) = 0;
};

} // namespace engine

#endif /* !A_UPDATE_SYSTEM_HPP_ */
