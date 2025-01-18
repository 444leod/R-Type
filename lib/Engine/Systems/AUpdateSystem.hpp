/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AUpdateSystem
*/

#ifndef A_UPDATE_SYSTEM_HPP_
#define A_UPDATE_SYSTEM_HPP_

#include <chrono>
#include <Engine/Systems/ASystem.hpp>

namespace engine
{

class AUpdateSystem : public engine::ASystem
{
public:
    explicit AUpdateSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute an 'update' system's logic
     * @param deltaTime The time between the last frame and the current one
     */
    void execute(double deltaTime)
    {
        const auto start = std::chrono::_V2::system_clock::now();
        this->_execution(deltaTime);
        const auto end = std::chrono::_V2::system_clock::now();
        _setExecutionTime(_name, std::chrono::duration_cast<std::chrono::duration<float, std::micro>>(end - start).count());
    }

protected:
    /**
     * @brief Execute an 'update' system's logic
     * @param deltaTime The time between the last frame and the current one
     */
    virtual void _execution(double deltaTime) = 0;

};

} // namespace engine

#endif /* !A_UPDATE_SYSTEM_HPP_ */
