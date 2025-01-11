/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** IScene.hpp
*/

#ifndef ASCENE_HPP
#define ASCENE_HPP

class SceneManager;

#include <map>
#include <vector>
#include <memory>

#include "RestrictedSceneManager.hpp"
#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include "ecs/Registry.hpp"
#include "ecs/Family.hpp"
#include "modules/ASceneModule.hpp"

namespace engine
{

    template <typename T>
    concept SceneModule = std::is_base_of_v<ASceneModule, T>;

    template <typename T, typename... Params>
    concept ConstructibleSceneModule = std::constructible_from<T, AScene&, Params...>;

}

class AScene {
public:
    AScene(RestrictedSceneManager& manager, ecs::Registry& registry, const std::string& name) :
        _manager(manager), _registry(registry), _name(name) {}

    virtual~AScene() = default;

    /**
     * @brief Called once at the start of the program
     */
    virtual void initialize() = 0;

    /**
     * @brief Called every frame
     * @param deltaTime The time between this frame and the last
     */
    virtual void update(const double& deltaTime) = 0;

    template<engine::SceneModule T>
    [[nodiscard]] std::shared_ptr<T> getModule()
    {
        for (auto module: this->_modules) {
            auto cast = std::dynamic_pointer_cast<T>(module);
            if (cast != nullptr)
            {
                return cast;
            }
        }
        return nullptr;
    }

    /**
     * @brief Gets the name of the scene
     * @return The name of the scene
     */
    [[nodiscard]] virtual const std::string& name() const noexcept { return this->_name; }

    /**
     * @brief Called when the Scene starts without a predecessor
     */
    virtual void onEnter() = 0;

    /**
     * @brief Called when the Scene starts with a predecessor
     * @param lastScene A reference to the previous scene
     */
    virtual void onEnter(const AScene& lastScene) = 0;

    /**
     * @brief Called when the Scene shuts with no successor
     */
    virtual void onExit() = 0;

    /**
     * @brief Called when the Scene shuts with a successor
     * @param nextScene A reference to the successor
     */
    virtual void onExit(const AScene& nextScene) = 0;

    void flush() const
    {
        this->_registry.flush();
    }

    /**
     * @brief Enables a system
     * @param systemName The name of the system to enable
     * @return true if the system was found and enabled, false otherwise
     */
    bool enableSystem(const std::string& systemName)
    {
        for (const auto& system : this->_updateSystems) {
            if (system->name() == systemName) {
                system->enable();
                return true;
            }
        }
        for (const auto& system : this->_renderSystems) {
            if (system->name() == systemName) {
                system->enable();
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Disables an update system
     * @param systemName The name of the system to disable
     * @return true if the system was found and disabled, false otherwise
     */
    bool disableSystem(const std::string& systemName)
    {
        for (const auto& system : this->_updateSystems) {
            if (system->name() == systemName) {
                system->disable();
                return true;
            }
        }
        for (const auto& system : this->_renderSystems) {
            if (system->name() == systemName) {
                system->disable();
                return true;
            }
        }
        return false;
    }

    template <engine::SceneModule T, typename... Params>
        requires engine::ConstructibleSceneModule<T, Params...>
    std::shared_ptr<T> addModule(Params&&... params)
    {
        auto module = std::make_shared<T>(*this, std::forward<Params>(params)...);
        this->_modules.push_back(module);
        return module;
    }


protected:

protected:
    ecs::Registry& _registry;
    RestrictedSceneManager& _manager;

    std::vector<std::unique_ptr<AUpdateSystem>> _updateSystems;
    std::vector<std::unique_ptr<ARenderSystem>> _renderSystems;

    /**
     * @brief Executes all the update systems in the scene
     * @param deltaTime The time between this frame and the last
     */
    void _executeUpdateSystems(const double& deltaTime)
    {
        for (const auto& system : this->_updateSystems) {
            if (system->isEnabled()) {
                system->execute(deltaTime);
            }
        }
    }

    /**
     * @brief Executes all the render systems in the scene
     * @param window The window to render to
     */
    void _executeRenderSystems(sf::RenderWindow& window)
    {
        for (const auto& system : this->_renderSystems) {
            if (system->isEnabled()) {
                system->execute(window);
            }
        }
    }


private:
    std::vector<std::shared_ptr<ASceneModule>> _modules;
    const std::string& _name = "";
};

#endif //ASCENE_HPP
