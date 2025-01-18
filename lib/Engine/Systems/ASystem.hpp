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

#include <map>
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

    /**
     * @brief Gets a reference of all the systems execution times
     * @return The execution times
     */
    [[nodiscard]] static const std::map<std::string, double>& executionTimes() { return _executionTimes; }

protected:
    void _setExecutionTime(const std::string& system, double time) {
        _executionTimes[system] = time;
    }

protected:
    ecs::Registry& _registry;
    bool _enabled = true;
    const std::string _name;

private:
    static std::map<std::string, double> _executionTimes; // this is like a big time registry
};

} // namespace engine

// Default declaration
inline std::map<std::string, double> engine::ASystem::_executionTimes = {};

#endif // A_SYSTEM_HPP
