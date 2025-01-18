/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameProfilingModule
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <exception>
#include <ranges>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "ECS/Registry.hpp"
#include "Engine/RestrictedGame.hpp"
#include "Engine/RestrictedSceneManager.hpp"
#include "Engine/AScene.hpp"
#include "Engine/Modules/AGameModule.hpp"

#define P_STR(x) std::to_string(x)

/**
 * @brief The GameProfilingModule adds an external window
 * that displays many kind of information about the currently running game that is using the R-Type engine.
 * @note The module itself does not rely on the engine to display information on screen, this is standalone.
 * @note A font is required for this module to work, as it will display text information.
 */
class GameProfilingModule final: public engine::AGameModule
{
public:
    /**
     * @warning To add a module to a game, use the `Game::addModule` method
     * @brief Creates a new GameProfilingModule
     * @param path The path to the font-file to load to display text information
     * @param updateJumps The amount of update it will jump before displaying new information
     */
    GameProfilingModule(const std::string& path, uint32_t updateJumps = 10): _jumps(updateJumps)
    {
        this->_lastUpdate = std::chrono::high_resolution_clock::now();
        if (!this->_font.loadFromFile(path))
            throw std::runtime_error("Failed to load font for Profiler.");
    }

    ~GameProfilingModule() = default;

public:
    /**
     * @brief Called when the module starts
     * @param scene The scene it is referncing
     */
    virtual void start([[maybe_unused]] engine::AScene& scene)
    {
        this->_window.create(sf::VideoMode(500, 350), "R-Type Profiling");
        if (!this->_window.isOpen())
            throw std::runtime_error("Failed to open window for Profiler.");
        this->_window.setFramerateLimit(15);
    }

    /**
     * @brief Unused
     */
    virtual void refresh([[maybe_unused]] engine::AScene& scene) {};

    /**
     * @brief Called when the module will be stopped
     */
    virtual void stop()
    {
        this->_window.close();
    }

    /**
     * @brief Called every tick, like any other module
     */
    virtual void update()
    {
        if (!this->_window.isOpen())
            return;

        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration_cast<std::chrono::duration<uint32_t, std::milli>>(now - this->_lastUpdate).count();
        this->_tickUpdates.push_back(delta);
        this->_lastUpdate = now;

        this->_ticks++;
        if (this->_ticks < this->_jumps)
            return;
        this->_ticks = 0;

        static const auto clear = sf::Color(255, 255, 255);
        this->_window.clear(clear);
        this->_events();

        // Draw Content
        this->_runtime();
        this->_updates();
        this->_scene();
        this->_entities();
        this->_components();
        this->_systems();
        //

        this->_window.display();
    }

private:
    /**
     * @brief Handles the different events of the profiling render window
     */
    void _events() {
        sf::Event event;
        while (this->_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->_window.close();
            if (event.type == sf::Event::MouseWheelMoved) {
                this->_scroll += event.mouseWheel.delta * 15.f;
                if (this->_scroll > 0.f) this->_scroll = 0.f;
                if (this->_scroll < _maxScroll) this->_scroll = this->_maxScroll;
            }
        }
    }

    /**
     * @brief Draws a text with a grey title (key) and a black text (value) at a given position
     * @param key The key text (in grey)
     * @param value The value text (in black)
     * @param x The x component of the coordinate
     * @param y The y component of the coordinate
     */
    void _drawKeyValue(const std::string& key, const std::string& value, float x, float y) {
        static sf::Text t_key;
        static sf::Text t_value;
        static const sf::Color c_key(110, 110, 110);
        static const sf::Color c_value(0, 0, 0);

        t_key.setString(key + ": ");
        t_key.setFont(this->_font);
        t_key.setCharacterSize(20);
        t_key.setFillColor(c_key);
        const auto w_key = t_key.getLocalBounds().width;

        t_value.setString(value);
        t_value.setFont(this->_font);
        t_value.setCharacterSize(20);
        t_value.setFillColor(c_value);
        const auto w_value = t_value.getLocalBounds().width;

        if (y > this->_window.getSize().y)
            this->_maxScroll = -(y - this->_window.getSize().y + 5); // 5px is arbitrary, it makes bottom text more readable
        y += this->_scroll;
        if (x >= 0.f) {
            t_key.setPosition(x, y);
            t_value.setPosition(x + w_key, y);
        } else {
            t_key.setPosition(this->_window.getSize().x - w_value - w_key + x, y );
            t_value.setPosition(this->_window.getSize().x - w_value + x, y);
        }
        this->_window.draw(t_key);
        this->_window.draw(t_value);
    }

private:

    /**!
     *          ALL THE FUNCTIONS BELOW REFER TO THE
     *          QUERYING AND THE DRAWING OF A SPECIFIC
     *          PART / VALUE TO BE DISPLAYED
     */

    // Runtime
    std::chrono::_V2::system_clock::time_point _startTime = std::chrono::high_resolution_clock::now();
    void _runtime() {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::duration<uint32_t, std::milli>>(now - this->_startTime).count();
        this->_drawKeyValue("Runtime", P_STR(duration) + "ms", 0, 0);
    }

    // Updates
    void _updates() {
        double fastest = 0.f;
        double slowest = 0.f;
        double avg = 0.f;
        double tps = 0.f;
        for (auto time: this->_tickUpdates) {
            if (time < fastest || fastest <= 0.f)
                fastest = time;
            if (time > slowest)
                slowest = time;
            avg += time;
        }
        if (this->_tickUpdates.size() != 0)
            avg /= (double)this->_tickUpdates.size();
        if (avg != 0.f)
            tps = 1000.f / avg;
        this->_tickUpdates.clear();
        std::stringstream ss;
        ss << "~" << avg << "ms, -" << fastest << "ms, +" << slowest << "ms (" << tps << "TPS)";
        this->_drawKeyValue("Updates", ss.str(), 0, 20);
    }

    // Entities
    void _entities() {
        const auto& reg = engine::RestrictedGame::instance().registry();
        const auto& ents = reg.entities();
        std::stringstream ss;
        ss << ents.size() << "/" << ents.capacity() << " (+" << reg.instances() << " -" << reg.deletions() << ")";
        this->_drawKeyValue("Entities", ss.str(), 0, 40);
    }

    // Scene
    void _scene() {
        const auto& scene = engine::RestrictedGame::instance().scenes().current();
        scene.name();
        this->_drawKeyValue("Scene", scene.name(), -5, 0);
    }

    // Components
    void _components() {
        const auto& sets = engine::RestrictedGame::instance().registry().sparseSets();
        this->_drawKeyValue("Kinds", P_STR(sets.size()),
        -5, 60);

        uint32_t size = 0;
        uint32_t capacity = 0;
        std::vector<std::pair<std::string, uint32_t>> frequencies = {};
        for (const auto [key, set]: sets) {
            size += set->size();
            frequencies.push_back({set->type(), set->size()});
            capacity += set->capacity();
        }
        this->_drawKeyValue("Components Total", P_STR(size) + "/" + P_STR(capacity), 0, 60);

        std::sort(frequencies.begin(), frequencies.end(), [](const auto& a, const auto& b){ return a.second > b.second; });
        std::stringstream ss;
        for (uint8_t i = 0; i < frequencies.size() && i < 3; i++) {
            if (i > 0) ss << ", ";
            ss << frequencies.at(i).first << " (" << frequencies.at(i).second << ")";
        }
        this->_drawKeyValue("Most Used Comp.s", ss.str(), 0, 80);
    }

    // Update Systems
    void _systems() {
        const auto& times = engine::ASystem::executionTimes();
        std::vector<std::pair<std::string, double>> frequencies = {};
        for (auto entry: times)
            frequencies.push_back(entry);
        std::ranges::sort(frequencies, [](const auto& a, const auto& b){ return a.second > b.second; });

        std::uint32_t y = 120;
        this->_drawKeyValue("Systems' execution times", "", 0, y);
        for (const auto& [system, time]: frequencies) {
            y += 20;
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << time << "us";
            this->_drawKeyValue(" - " + system, ss.str(), 0, y);
        }
    }

private:
    sf::Font _font;
    sf::RenderWindow _window;
    float _scroll = 0.f;
    float _maxScroll = 0.f;

    uint32_t _ticks;
    const uint32_t _jumps;
    std::chrono::_V2::system_clock::time_point _lastUpdate;
    std::vector<double> _tickUpdates = {};
};
