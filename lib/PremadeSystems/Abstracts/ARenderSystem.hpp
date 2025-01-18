/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ARenderSystem
*/

#ifndef A_RENDER_SYSTEM_HPP_
#define A_RENDER_SYSTEM_HPP_

#include <Engine/Systems/ASystem.hpp>
#include <SFML/Graphics.hpp>

class ARenderSystem : public engine::ASystem
{
  public:
    explicit ARenderSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute a 'render' system's logic
     * @param window The window to render to
     */
    void execute(sf::RenderWindow& window)
    {
        const auto start = std::chrono::_V2::system_clock::now();
        this->_execution(window);
        const auto end = std::chrono::_V2::system_clock::now();
        _setExecutionTime(_name, std::chrono::duration_cast<std::chrono::duration<float, std::micro>>(end - start).count());
    }

protected:
    /**
     * @brief Execute a 'render' system's logic
     * @param window The window to render to
     */
    virtual void _execution(sf::RenderWindow& window) = 0;
};

#endif /* !A_RENDER_SYSTEM_HPP_ */
