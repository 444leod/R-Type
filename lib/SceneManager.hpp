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
#include "IScene.hpp"

namespace scene {
    template<typename T>
    concept SceneType = std::is_base_of_v<IScene, T>;
}

class SceneManager final {
    class SceneManagerError final : public std::exception {
        public:
            explicit SceneManagerError(std::string message) : _message(std::move(message)) {}
            ~SceneManagerError() override = default;

            [[nodiscard]] const char *what() const noexcept override
            {
                return _message.c_str();
            }

        private:
            std::string _message;
    };

public:
    SceneManager() = default;
    virtual ~SceneManager() = default;

    template <scene::SceneType T>
    void registerScene(const std::string &name)
    {
        if (name.empty())
            throw SceneManagerError("Scene name cannot be empty");
        std::shared_ptr<T> scene = std::make_shared<T>(_currentSceneName, _running);
        scene->initialize();
        _scenes[name] = scene;
    }

    void reset()
    {
        _current = nullptr;
        _currentSceneName = "";
        _oldSceneName = "";
    }

    void run(const std::string& sceneName)
    {
        auto before = std::chrono::high_resolution_clock::now();

        _currentSceneName = sceneName;
        _oldSceneName = sceneName;
        _current = _scenes[sceneName];

        while (_running) {
            auto now = std::chrono::high_resolution_clock::now();
            const double deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - before).count() / 1000.0;
            before = now;
            _updateSceneState();
            _update(deltaTime);
            _pollEvents();
            _render();
        }
    }

private:
    void _switchTo(const std::string& scene)
    {
        if (!_current)
            throw SceneManagerError("No current scene");
        if (!_scenes.contains(scene)  )
            throw SceneManagerError("Scene " + scene + " not found");

        _current->onExit(scene);
        _current = _scenes[scene];
        _current->onEnter(_oldSceneName);
        _currentSceneName = scene;
        _oldSceneName = scene;
    }

    void _updateSceneState()
    {
        if (_currentSceneName != _oldSceneName) {
            this->_switchTo(_currentSceneName);
        }
    }

    void _update(const double deltaTime) const
    {
        _current->update(deltaTime);
    }

    void _pollEvents()
    {
        sf::Event event{};
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                _window.close();
            _current->onEvent(event);
        }
    }

    void _render()
    {
        _window.clear();
        _current->render(_window);
        _window.display();
    }

private:
    bool _running = true;

    std::map<std::string, std::shared_ptr<IScene>> _scenes = {};
    std::shared_ptr<IScene> _current = nullptr;
    std::string _currentSceneName;
    std::string _oldSceneName;

    sf::RenderWindow _window;
};



#endif //SCENEMANAGER_HPP
