/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <Engine/Modules/AGameModule.hpp>
#include <Engine/RestrictedGame.hpp>
#include <Engine/SceneManager.hpp>

#include "RestrictedGame.hpp"
#include "SceneManager.hpp"

#include "ECS/EventDispatcher.hpp"
#include "ECS/Registry.hpp"

#include <memory>
#include <thread>

namespace engine
{
template <typename T>
concept GameModule = std::is_base_of_v<AGameModule, T>;

template <typename T, typename... Params>
concept ConstructibleGameModule = std::constructible_from<T, Params...>;

class Game final : public RestrictedGame
{
  public:
    Game() = default;
    ~Game() override = default;

    [[nodiscard]] const ecs::Registry& registry() const override { return this->_registry; }
    [[nodiscard]] ecs::Registry& registry() override { return this->_registry; }

    [[nodiscard]] const ecs::EventDispatcher& eventDispatcher() const override { return this->_events; }
    [[nodiscard]] ecs::EventDispatcher& eventDispatcher() override { return this->_events; }

    [[nodiscard]] const SceneManager& scenes() const override { return this->_sceneManager; }
    [[nodiscard]] SceneManager& scenes() override { return this->_sceneManager; }

    template <GameModule Module, typename... Params>
        requires ConstructibleGameModule<Module, Params...>
    std::shared_ptr<Module> addModule(Params&&... params)
    {
        auto module = std::make_shared<Module>(std::forward<Params>(params)...);
        this->_modules.push_back(module);
        return module;
    }

    template <SceneType T> std::shared_ptr<T> registerScene(const std::string& name) { return this->_sceneManager.registerScene<T>(name); }

    void run() { this->run(this->_sceneManager.loaded()); }

    void run(const std::string& scene)
    {
        this->_running = true;
        this->_sceneManager.load(scene);
        this->_sceneManager.update();
        auto before = std::chrono::high_resolution_clock::now();
        double deltaTime = .0;

        for (const auto& module : this->_modules)
        {
            module->start(this->_sceneManager.current());
            module->refresh(this->_sceneManager.current());
        }

        while (_running)
        {
            if (this->_sceneManager.update())
                for (const auto& module : this->_modules)
                    module->refresh(this->_sceneManager.current());
            for (const auto& module : this->_modules)
                module->update();
            this->_sceneManager.current().update(deltaTime);
            this->_registry.flush();

            // Compute deltaTime
            auto now = std::chrono::high_resolution_clock::now();
            deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - before).count() / 1000.0;
            if (deltaTime < this->_targetDeltaTime)
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(this->_targetDeltaTime - deltaTime)));
            before = now;
        }

        this->_sceneManager.stop();
        for (const auto& module : this->_modules)
            module->stop();
    }

    /**
     * @brief Swiftly stops the game from running
     */
    void stop() noexcept override { this->_running = false; }

  private:
    ecs::Registry _registry;
    ecs::EventDispatcher _events;

    std::vector<std::shared_ptr<AGameModule>> _modules;
    SceneManager _sceneManager;

    double _targetDeltaTime = 1000.0 / 60.0;
    bool _running = false;
};
} // namespace engine

#endif // GAME_HPP
