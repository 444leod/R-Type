/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** IScene.hpp
*/

#ifndef ASCENE_HPP
#define ASCENE_HPP

class SceneManager;

#include <SFML/Graphics.hpp>
#include "UDPPacket.hpp"
#include "ISceneManager.hpp"
#include "Registry.hpp"
#include "BaseSystems/Abstracts/AUpdateSystem.hpp"
#include "BaseSystems/Abstracts/ARenderSystem.hpp"
#include <vector>
#include <memory>

class AScene {
public:
    AScene(ISceneManager& manager, const std::string& name) :
        _manager(manager), _name(name) {}
    virtual~AScene() = default;

    /**
     * @brief Called once at the start of the program
     */
    virtual void initialize() = 0;

    /**
     * @brief Called every frame
     * @param deltaTime The time between this frame and the last
     * @param window The window to render to
     */
    virtual void update(double deltaTime, const sf::RenderWindow &window) = 0;

    /**
     * @brief Used to render the elements in a scene
     * @param window The  window to render to
     */
    virtual void render(sf::RenderWindow &window) = 0;

    /**
     * @brief Gets the name of the scene
     * @return The name of the scene
     */
    virtual const std::string& name() const noexcept { return this->_name; }

    /**
     * @brief Called for every window event.
     * @param event The event to be taken care of
     */
    virtual void onEvent(sf::Event &event) = 0;

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

    virtual void onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) = 0;

    void flush()
    {
        this->_registry.flush();
    }

    /**
     * @brief Enables a system
     * @param systemName The name of the system to enable
     * @return true if the system was found and enabled, false otherwise
     */
    bool enableSystem(const std::string systemName)
    {
        for (auto& system : this->_updateSystems) {
            if (system->name() == systemName) {
                system->enable();
                return true;
            }
        }
        for (auto& system : this->_renderSystems) {
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
    bool disableSystem(const std::string systemName)
    {
        for (auto& system : this->_updateSystems) {
            if (system->name() == systemName) {
                system->disable();
                return true;
            }
        }
        for (auto& system : this->_renderSystems) {
            if (system->name() == systemName) {
                system->disable();
                return true;
            }
        }
        return false;
    }


protected:
    /**
     * @brief Executes all the update systems in the scene
     * @param deltaTime The time between this frame and the last
     * @param window The window used as a reference for the system for logic, not for rendering
     */
    void _executeUpdateSystems(double deltaTime, const sf::RenderWindow &window)
    {
        for (auto& system : this->_updateSystems) {
            if (system->isEnabled()) {
                system->execute(deltaTime, window);
            }
        }
    }

    /**
     * @brief Executes all the render systems in the scene
     * @param window The window to render to
     */
    void _executeRenderSystems(sf::RenderWindow& window)
    {
        for (auto& system : this->_renderSystems) {
            if (system->isEnabled()) {
                system->execute(window);
            }
        }
    }

    ISceneManager& _manager;
    Registry _registry;

    std::vector<std::unique_ptr<AUpdateSystem>> _updateSystems;
    std::vector<std::unique_ptr<ARenderSystem>> _renderSystems;

private:
    const std::string& _name = "";
};

#endif //ASCENE_HPP
