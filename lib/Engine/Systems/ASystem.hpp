/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ASystem.hpp
*/

#ifndef A_SYSTEM_HPP
#define A_SYSTEM_HPP

#include <ECS/Registry.hpp>
#include <Engine/RestrictedGame.hpp>

#include <string>
#include <utility>

namespace engine
{
class ASystem
{
  public:
    explicit ASystem(std::string name) : _registry(engine::RestrictedGame::instance().registry()), _name(std::move(name)) {}
    virtual ~ASystem() = default;

    /**
     * @brief Disables the system
     */
    void disable() { _enabled = false; }

    /**
     * @brief Enables the system
     */
    void enable() { _enabled = true; }

    /**
     * @brief Returns whether the system is enabled
     * @return Whether the system is enabled
     */
    [[nodiscard]] bool isEnabled() const { return _enabled; }

    /**
     * @brief Returns the system's name
     * @return The system's name
     */
    [[nodiscard]] std::string name() const { return _name; }

  protected:
    ecs::Registry& _registry;
    bool _enabled = true;
    const std::string _name;
};

} // namespace engine

#endif // A_SYSTEM_HPP
