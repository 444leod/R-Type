/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game
*/

#pragma once

#include "SceneManager.hpp"
#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"
#include "modules/IGameModule.hpp"

class Game
{
public:
    Game() = default;
    ~Game()
    {
        for (auto module: this->_modules)
            delete module;
    }

    const ecs::Registry& registry() const { return this->_registry; }
    ecs::Registry& registry() { return this->_registry; }

    const ecs::EventDispatcher& eventDispatcher() const { return this->_events; }
    ecs::EventDispatcher& eventDispatcher() { return this->_events; }

    const SceneManager& scenes() const { return this->_sceneManager; }
    SceneManager& scenes() { return this->_sceneManager; }


    // constraint to IGameModule
    template<typename T, typename... Params>
    void addModule(Params... params)
    {
        IGameModule *module = new T(params...);
        module->start();
        this->_modules.push_back(module);
    }

    template<scene::SceneType T>
    void registerScene(const std::string &name)
    {
        this->_sceneManager.registerScene<T>(name, this->_registry);
    }

    void run() {
        this->run(this->_sceneManager.firstRegistered());
    }

    void run(const std::string& scene) {
        this->_running = true;
        this->_sceneManager.load(scene);
        auto before = std::chrono::high_resolution_clock::now();
        double deltaTime = .0;

        while (_running) {

            this->_sceneManager.update();
            for (auto module: this->_modules)
                module->update(*this);
            this->_sceneManager.current().update(deltaTime);
            this->_registry.flush();

            // Compute deltaTime
            auto now = std::chrono::high_resolution_clock::now();
            before = now;
            deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - before).count() / 1000.0;
            if (deltaTime < this->_targetDeltaTime)
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(this->_targetDeltaTime - deltaTime)));

        }
    }

    /**
     * @brief Switfly stops the game from running
     */
    void stop()
    {
        this->_running = false;
        this->_sceneManager.stop();
        for (auto module: this->_modules)
            module->stop();
    }

private:
    std::vector<IGameModule *> _modules;
    ecs::Registry _registry;
    ecs::EventDispatcher _events;
    SceneManager _sceneManager;
    double _targetDeltaTime = 1000.0 / 60.0;
    bool _running = false;
};
