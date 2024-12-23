/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** SceneManager.hpp
*/

#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <memory>
#include <map>
#include <chrono>

#include <utility>
#include <exception>
#include <thread>
#include <asio.hpp>
#include <iostream>

#include "ISceneManager.hpp"
#include "AScene.hpp"
#include "config.h"

namespace ecs {
    class Registry;
}


namespace scene {
    /**
     * @brief Concept to ensure the type is derived from AScene.
     */
    template<typename T>
    concept SceneType = std::is_base_of_v<AScene, T>;
}

/**
 * @class SceneManager
 * @brief Manages the scenes in the application.
 */
class SceneManager final : public ISceneManager {
public:
    /**
     * @class Exception
     * @brief Custom exception class for SceneManager.
     */
    class Exception final : public std::exception {
    public:
        /**
         * @brief Constructor with a message.
         * @param message The exception message.
         */
        explicit Exception(std::string message) : _message(std::move(message)) {}

        /**
         * @brief Default destructor.
         */
        ~Exception() override = default;

        /**
         * @brief Returns the exception message.
         * @return const char * The exception message.
         */
        [[nodiscard]] const char *what() const noexcept override { return _message.c_str(); }
    private:
        std::string _message; ///< The exception message.
    };

    SceneManager() = default;
    ~SceneManager() override = default;

    /**
     * @brief Registers a scene with a given name.
     * @tparam T The type of the scene.
     * @param name The name of the scene.
     * @throws Exception if the scene name is empty.
     */
    template <scene::SceneType T>
    void registerScene(const std::string &name, ecs::Registry& registry)
    {
        if (name.empty())
            throw Exception("Scene name cannot be empty");
        std::shared_ptr<T> scene = std::make_shared<T>(*this, registry, name);
        scene->initialize();
        this->_scenes[name] = scene;
    }

    /**
     * @brief Resets the scene manager.
     */
    void reset()
    {
        this->_current = nullptr;
        this->_loadingName = "";
    }

    /**
     * @brief Loads a scene by name.
     * @param name The name of the scene to load.
     */
    void load(const std::string& name) override
    {
        if (!this->_running)
            return;
        if (this->_scenes.contains(name))
            this->_loadingName = name;
    }

    /**
     * @brief Stops the scene manager.
     */
    void stop() noexcept override
    {
        this->_running = false;
        if (this->_current != nullptr)
            this->_current->onExit();
    }

    /**
     * @brief Gets the first loaded Scene in the registry, throws if there is none
     * @return A const reference to the name of the first loaded Qcene
     */
    const std::string& firstRegistered() {
        if (this->_scenes.empty())
            throw Exception("No scenes currently registered.");
        return this->_scenes.begin()->first;
    }

    /**
     * @brief Updates the state of the current scene.
     */
    void update()
    {
        if (_loadingName.empty())
            return;

        /// Exit the current scene if there was one
        const auto next = this->_scenes[this->_loadingName];
        if (this->_current != nullptr)
            this->_current->onExit(*next);
        /// Load the next scene
        const auto old = this->_current;
        this->_current = next;
        old != nullptr
            ? this->_current->onEnter(*old)
            : this->_current->onEnter();
        this->_loadingName = "";
    }

    /**
     * @brief Gets the currently running scene
     * @return A refernece to the currently running scene
     */
    AScene& current()
    {
        if (_current == nullptr)
            throw Exception("No scenes currently running.");
        return *this->_current;
    }

private:
    bool _running = true;

    std::map<std::string, std::shared_ptr<AScene>> _scenes = {};
    std::shared_ptr<AScene> _current = nullptr;
    std::string _loadingName;
};

#endif //SCENEMANAGER_HPP
