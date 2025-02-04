/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** RestrictedSceneManager.hpp
*/

#ifndef RESTRICTED_SCENE_MANAGER_HPP
#define RESTRICTED_SCENE_MANAGER_HPP

#include <string>
#include <vector>
#include <memory>

namespace engine
{
// Forward declaration
class AScene;
class AGameModule;

/**
 * @class RestrictedSceneManager
 * @brief Interface for managing scenes in the application.
 */
class RestrictedSceneManager
{
  public:
    RestrictedSceneManager(std::vector<std::shared_ptr<AGameModule>>& modules) : _modules(modules) {}
    virtual ~RestrictedSceneManager() = default;

    /**
     * @brief Stops the scene manager.
     */
    virtual void stop() noexcept = 0;

    /**
     * @brief Loads a scene by name.
     * @param name The name of the scene to load.
     */
    virtual void load(const std::string& name) = 0;

    /**
     * @brief Retrieves a scene by name.
     *
     * @param name The name of the scene to retrieve.
     */
    virtual AScene& get(const std::string& name) = 0;

    /**
     * @brief Retrieves the currently running scene.
     */
    virtual AScene& current() = 0;

protected:
    std::vector<std::shared_ptr<AGameModule>>& _modules;
};

} // namespace engine

#endif // RESTRICTED_SCENE_MANAGER_HPP
