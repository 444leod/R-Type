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
#include <SFML/Graphics.hpp>
#include <utility>
#include <exception>
#include "ISceneManager.hpp"
#include "AScene.hpp"

namespace scene {
    template<typename T>
    concept SceneType = std::is_base_of_v<AScene, T>;
}

class SceneManager final : public ISceneManager {
public:
    class Exception final : public std::exception {
        public:
            explicit Exception(std::string message) : _message(std::move(message)) {}
            ~Exception() override = default;
            [[nodiscard]] const char *what() const noexcept override { return _message.c_str(); }
        private:
            std::string _message;
    };

    SceneManager() = default;
    ~SceneManager() override = default;

    template <scene::SceneType T>
    void registerScene(const std::string &name)
    {
        if (name.empty())
            throw Exception("Scene name cannot be empty");
        std::shared_ptr<T> scene = std::make_shared<T>(*this, name);
        scene->initialize();
        this->_scenes[name] = scene;
    }

    void reset()
    {
        this->_current = nullptr;
        this->_loadingName = "";
    }

    void load(const std::string& name)
    {
        if (!this->_running)
            return;
        if (this->_scenes.contains(name))
            this->_loadingName = name;
        // could just throw in case the scene doesn't exist
    }

    void run()
    {
        auto before = std::chrono::high_resolution_clock::now();

        if (this->_loadingName == "")
            throw Exception("No scenes curretly loaded.");
        while (_running) {
            auto now = std::chrono::high_resolution_clock::now();
            const double deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - before).count() / 1000.0;
            before = now;
            this->_updateSceneState();
            this->_pollEvents();
            this->_current->update(deltaTime);
            this->_render();
        }
    }

    void stop() noexcept override{ this->_running = false; }

private:
    void _updateSceneState()
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

    void _pollEvents()
    {
        sf::Event event{};
        while (this->_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->_window.close();
            this->_current->onEvent(event);
        }
    }

    void _render()
    {
        this->_window.clear();
        this->_current->render(_window);
        this->_window.display();
    }

private:
    bool _running = true;

    std::map<std::string, std::shared_ptr<AScene>> _scenes = {};
    std::shared_ptr<AScene> _current = nullptr;
    std::string _loadingName;

    sf::RenderWindow _window;
};

#endif //SCENEMANAGER_HPP
