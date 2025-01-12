/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** SceneManager.hpp
*/

#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <chrono>
#include <map>
#include <memory>

#ifndef __RTYPE_NO_DISPLAY__
#include <SFML/Graphics.hpp>
#endif

#include "AScene.hpp"
#include "ISceneManager.hpp"
#include "NetworkedScene.hpp"
#include "config.h"
#include <asio.hpp>
#include <exception>
#include <thread>
#include <utility>

#include <iostream>

namespace scene {

constexpr int TARGET_FPS = 60;
const std::chrono::milliseconds FRAME_DURATION(1000 / TARGET_FPS);

/**
 * @brief Concept to ensure the type is derived from AScene.
 */
template <typename T>
concept SceneType = std::is_base_of_v<AScene, T>;
} // namespace scene

/**
 * @class SceneManager
 * @brief Manages the scenes in the application.
 */
class SceneManager final : public ISceneManager, public NetworkAgent {
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
        [[nodiscard]] const char* what() const noexcept override { return _message.c_str(); }

      private:
        std::string _message; ///< The exception message.
    };

    explicit SceneManager(asio::io_context& io_context, const int& port = 0) : NetworkAgent(io_context, port) { _window.setKeyRepeatEnabled(false); }
    ~SceneManager() override = default;

    /**
     * @brief Registers a scene with a given name.
     * @tparam T The type of the scene.
     * @param name The name of the scene.
     * @throws Exception if the scene name is empty.
     */
    template <scene::SceneType T> void registerScene(const std::string& name) {
        if (name.empty())
            throw Exception("Scene name cannot be empty");
        std::shared_ptr<T> scene = std::make_shared<T>(*this, name);
        scene->initialize();
        this->_scenes[name] = scene;
    }

    /**
     * @brief Resets the scene manager.
     */
    void reset() {
        this->_current = nullptr;
        this->_loadingName = "";
    }

    /**
     * @brief Loads a scene by name.
     * @param name The name of the scene to load.
     */
    void load(const std::string& name) override {
        if (!this->_running)
            return;
        if (this->_scenes.contains(name))
            this->_loadingName = name;
    }

    /**
     * @brief Runs the scene manager.
     * @throws Exception if no scenes are currently loaded.
     */
    void run() {
        auto before = std::chrono::high_resolution_clock::now();

        if (this->_loadingName.empty())
            throw Exception("No scenes currently loaded.");
        while (_running) {
            auto now = std::chrono::high_resolution_clock::now();
            const double deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - before).count() / 1000.0;
            before = now;
            this->_updateSceneState();

#ifndef __RTYPE_NO_DISPLAY__
            this->_pollEvents();
#endif

            this->_current->update(deltaTime, _window);

#ifndef __RTYPE_NO_DISPLAY__
            this->_render();
            const double elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(std::chrono::high_resolution_clock::now() - now).count() / 1000.0;
            if (elapsed < scene::FRAME_DURATION.count())
                std::this_thread::sleep_for(scene::FRAME_DURATION - std::chrono::milliseconds(static_cast<int>(elapsed)));
#endif

            this->_current->flush();
        }
    }

    /**
     * @brief Stops the scene manager.
     */
    void stop() noexcept override {
        this->_running = false;
        if (this->_current != nullptr)
            this->_current->onExit();
    }

    /**
     * @brief Sends a packet to a destination.
     *
     * @param dest The destination to send the packet to.
     * @param packet The packet to send.
     */
    void send(const asio::ip::udp::endpoint& dest, const UDPPacket& packet) override { this->_send(dest, packet); }

  private:
    /**
     * @brief Updates the state of the current scene.
     */
    void _updateSceneState() {
        if (_loadingName.empty())
            return;

        /// Exit the current scene if there was one
        const auto next = this->_scenes[this->_loadingName];
        if (this->_current != nullptr)
            this->_current->onExit(*next);
        /// Load the next scene
        const auto old = this->_current;
        this->_current = next;
        old != nullptr ? this->_current->onEnter(*old) : this->_current->onEnter();
        this->_loadingName = "";
    }

#ifndef __RTYPE_NO_DISPLAY__

    /**
     * @brief Polls events from the window.
     */
    void _pollEvents() {
        sf::Event event{};
        while (this->_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->_window.close();
                this->stop();
                return;
            }
            this->_current->onEvent(event);
        }
    }

    /**
     * @brief Renders the current scene.
     */
    void _render() {
        this->_window.clear();
        this->_current->render(_window);
        this->_window.display();
    }

#endif

    void _onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) override {
        if (this->_current != nullptr) {
            this->_current->onPacketReceived(src, packet);
        }
    }

  private:
    bool _running = true;

    std::map<std::string, std::shared_ptr<AScene>> _scenes = {};
    std::shared_ptr<AScene> _current = nullptr;
    std::string _loadingName;

#ifndef __RTYPE_NO_DISPLAY__
    sf::RenderWindow _window = sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT* SCALE), "R-Type");
#endif
};

#endif // SCENEMANAGER_HPP
