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

protected:
    ISceneManager& _manager;
    Registry _registry;

private:
    const std::string& _name = "";
};

#endif //ASCENE_HPP
