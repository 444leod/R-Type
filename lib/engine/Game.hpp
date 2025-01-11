/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game
*/

#pragma once

#include "RestrictedGame.hpp"

#include "modules/AGameModule.hpp"

#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"
#include "SceneManager.hpp"

#include <memory>
#include <thread>

namespace game
{
    template <typename T>
    concept GameModule = std::is_base_of_v<AGameModule, T>;

    template <typename T, typename... Params>
    concept ConstructibleGameModule = std::constructible_from<T, RestrictedGame&, Params...>;

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
            auto module = std::make_shared<Module>(*this, std::forward<Params>(params)...);
            this->_modules.push_back(module);
            return module;
        }

        template<scene::SceneType T>
        std::shared_ptr<T> registerScene(const std::string &name)
        {
            return this->_sceneManager.registerScene<T>(name, this->_registry);
        }

        void run()
        {
            this->run(this->_sceneManager.loaded());
        }

        void run(const std::string& scene) {
            this->_running = true;
            this->_sceneManager.load(scene);
            auto before = std::chrono::high_resolution_clock::now();
            double deltaTime = .0;

            for (const auto& module: this->_modules)
                module->start();

            while (_running)
            {
                if (this->_sceneManager.update())
                    for (const auto& module: this->_modules)
                        module->refresh(this->_sceneManager.current());
                for (const auto& module: this->_modules)
                    module->update();
                this->_sceneManager.current().update(deltaTime);
                this->_registry.flush();

                // Compute deltaTime
                auto now = std::chrono::high_resolution_clock::now();
                before = now;
                deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - before).count() / 1000.0;
                if (deltaTime < this->_targetDeltaTime)
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(this->_targetDeltaTime - deltaTime)));
            }

            this->_sceneManager.stop();
            for (const auto& module: this->_modules)
              module->stop();
        }

        /**
         * @brief Swiftly stops the game from running
         */
        void stop() noexcept override
        {
            this->_running = false;
        }

    private:
        std::vector<std::shared_ptr<AGameModule>> _modules;
        ecs::Registry _registry;
        ecs::EventDispatcher _events;
        SceneManager _sceneManager;
        double _targetDeltaTime = 1000.0 / 60.0;
        bool _running = false;
    };
}