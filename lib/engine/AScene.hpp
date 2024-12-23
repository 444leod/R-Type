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
#include "ISceneManager.hpp"
#include "ecs/Registry.hpp"
#include "ecs/Family.hpp"
#include "modules/ISceneModule.hpp"

class AScene {
public:
    AScene(ISceneManager& manager, ecs::Registry& registry, const std::string& name) :
        _manager(manager), _registry(registry), _name(name) {}

    virtual~AScene()
    {
        for (auto [_, module]: this->_modules)
            delete module;
    }

    /**
     * @brief Called once at the start of the program
     */
    virtual void initialize() = 0;

    /**
     * @brief Called every frame
     * @param deltaTime The time between this frame and the last
     * @param window The window to render to
     */
    virtual void update(double deltaTime) = 0;

    template<typename T>
    T *getModule()
    {
        auto id = ecs::Family<T>::id();
        if (this->_modules.contains(id))
            return dynamic_cast<T *>(this->_modules.at(id));
        return nullptr;
    }

    /**
     * @brief Gets the name of the scene
     * @return The name of the scene
     */
    virtual const std::string& name() const noexcept { return this->_name; }

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

protected:
    template<typename T>
    void _addModule()
    {
        auto id = ecs::Family<T>::id();
        this->_modules[id] = new T();
    }

protected:
    ecs::Registry& _registry;
    ISceneManager& _manager;

private:
    std::map<std::uint32_t, ISceneModule *> _modules = {};
    const std::string& _name = "";
};

#endif //ASCENE_HPP
