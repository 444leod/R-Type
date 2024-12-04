/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ISceneManager.hpp
*/

#ifndef ISCENEMANAGER_HPP
#define ISCENEMANAGER_HPP

#include <string>

/**
 * @class ISceneManager
 * @brief Interface for managing scenes in the application.
 */
class ISceneManager {
public:
    ISceneManager() = default;
    virtual ~ISceneManager() = default;

    /**
     * @brief Stops the scene manager.
     */
    virtual void stop() noexcept = 0;

    /**
     * @brief Loads a scene by name.
     * @param name The name of the scene to load.
     */
    virtual void load(const std::string& name) = 0;
};

#endif //ISCENEMANAGER_HPP
