/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** SceneManager.hpp
*/

#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <Engine/RestrictedSceneManager.hpp>
#include <Engine/AScene.hpp>

#include <memory>
#include <map>
#include <chrono>
#include <utility>
#include <exception>

namespace engine
{

    /**
     * @brief Concept to ensure the type is derived from AScene.
     */
    template<typename T>
    concept SceneType = std::is_base_of_v<AScene, T>;

    /**
     * @class SceneManager
     * @brief Manages the scenes in the application.
     */
    class SceneManager final : public RestrictedSceneManager {
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
         *
         * @param name The name of the scene.
         *
         * @throws Exception if the scene name is empty.
         */
        template <SceneType T>
        std::shared_ptr<T> registerScene(const std::string &name)
        {
            if (name.empty())
                throw Exception("Scene name cannot be empty");
            std::shared_ptr<T> scene = std::make_shared<T>(name);
            scene->initialize();
            this->_scenes[name] = scene;
            if (this->_loadingName.empty())
                this->_loadingName = name;
            return scene;
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
         * @brief Updates the state of the current scene.
         *
         * @return true if the scene was updated, false otherwise.
         */
        bool update()
        {
            if (this->_loadingName.empty() || !this->_scenes.contains(this->_loadingName))
                return false;

            /// Exit the current scene if there was on e
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
            return true;
        }

        /**
         * @brief Gets the currently running scene
         * @return A reference to the currently running scene
         */
        AScene& current() override
        {
            if (_current == nullptr)
                throw Exception("No scenes currently running.");
            return *this->_current;
        }

        /**
         * @brief Gets a scene by name
         *
         * @param name The name of the scene to get
         *
         * @return A reference to the scene
         *
         * @exception Exception if the scene is not found
         */
        AScene& get(const std::string& name) override
        {
            if (!this->_scenes.contains(name))
                throw Exception("Scene not found.");
            return *this->_scenes[name];
        }

        [[nodiscard]] const std::string& loaded() const
        {
            return this->_loadingName;
        }

    private:
        bool _running = true;

        std::map<std::string, std::shared_ptr<AScene>> _scenes = {};
        std::shared_ptr<AScene> _current = nullptr;
        std::string _loadingName;
    };
} // namespace engine

#endif //SCENE_MANAGER_HPP
