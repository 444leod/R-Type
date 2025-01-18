/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AEventSystem.hpp
*/

#ifndef A_EVENT_SYSTEM_HPP
#define A_EVENT_SYSTEM_HPP

#include <chrono>
#include <Engine/Systems/ASystem.hpp>
#include <SFML/Graphics.hpp>

class AEventSystem : public engine::ASystem
{
  public:
    explicit AEventSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute a 'event' system's logic
     * @param event The event to handle
     */
    void execute(sf::Event& event)
    {
        const auto start = std::chrono::_V2::system_clock::now();
        this->_execution(event);
        const auto end = std::chrono::_V2::system_clock::now();
        _setExecutionTime(_name, std::chrono::duration_cast<std::chrono::duration<float, std::micro>>(end - start).count());
    }

protected:
    /**
     * @brief Execute a 'event' system's logic
     * @param event The event to handle
     */
    virtual void _execution(sf::Event& event) = 0;
};

#endif // A_EVENT_SYSTEM_HPP
