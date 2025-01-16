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

#include "AGameModule.hpp"

#define P_STR(x) std::to_string(x)

namespace engine
{
    class GameProfilingModule final: public AGameModule
    {
    public:
        GameProfilingModule(game::RestrictedGame& game, uint32_t updateJumps, const std::string& path): AGameModule(game), _jumps(updateJumps)
        {
            this->_lastUpdate = std::chrono::high_resolution_clock::now();
            if (!this->_font.loadFromFile(path))
                throw std::runtime_error("Failed to load font for Profiler.");
        }

        ~GameProfilingModule() = default;

    public:
        virtual void start(AScene& scene)
        {
            this->_window.create(sf::VideoMode(400, 200), "R-Type Profiling");
            if (!this->_window.isOpen())
                throw std::runtime_error("Failed to open window for Profiler.");
            this->_window.setFramerateLimit(15);
        }

        virtual void refresh(AScene& scene) {};

        virtual void stop()
        {
            this->_window.close();
        }

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

            static const auto clear = sf::Color(200, 200, 200);
            this->_window.clear(clear);
            this->_events();

            // Draw Content
            this->_runtime();
            this->_updates();
            this->_scene();
            this->_entities();
            //

            this->_window.display();
        }

    private:
        void _events() {
            sf::Event event;
            while (this->_window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    this->_window.close();
            }
        }

        void _drawKeyValue(const std::string& key, const std::string& value, float x, float y) {
            static sf::Text t_key;
            static sf::Text t_value;
            static const sf::Color c_key(55, 55, 55);
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

            if (x >= 0.f) {
                t_key.setPosition(x, y);
                t_value.setPosition(x + w_key, y);
            } else {
                t_key.setPosition(this->_window.getSize().x - w_value - w_key + x, y);
                t_value.setPosition(this->_window.getSize().x - w_value + x, y);
            }
            this->_window.draw(t_key);
            this->_window.draw(t_value);
        }

    private:

        // Runtime
        std::chrono::_V2::system_clock::time_point _startTime = std::chrono::high_resolution_clock::now();
        void _runtime() {
            const auto now = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::duration<uint32_t, std::milli>>(now - this->_startTime).count();
            this->_drawKeyValue("Runtime", P_STR(duration) + "ms", 0, 0);
        }

        // Updates
        void _updates() {
            double fastestUpdate = 0.f;
            double slowestUpdate = 0.f;
            double avg = 0.f;
            for (auto time: this->_tickUpdates) {
                if (time < fastestUpdate || fastestUpdate <= 0.f)
                    fastestUpdate = time;
                if (time > slowestUpdate)
                    slowestUpdate = time;
                avg += time;
            }
            avg /= (double)this->_tickUpdates.size();
            this->_tickUpdates.clear();
            this->_drawKeyValue("Updates",
                std::string("~") + P_STR((int)avg) + "ms, -" + P_STR((int)fastestUpdate) + "ms, +" + P_STR((int)slowestUpdate) + "ms",
                0, 20);
        }

        // Entities
        void _entities() {
            const auto& reg = _game.registry();
            const auto& ents = _game.registry().entities();
            this->_drawKeyValue("Entities",
                P_STR(ents.size()) + "/" + P_STR(ents.capacity()) +
                " (+" + P_STR(reg.instances()) + " -" + P_STR(reg.deletions()) + ")",
                0, 40);
        }

        // Scene
        void _scene() {
            const auto& scene = _game.scenes().current();
            scene.name();
            this->_drawKeyValue("Scene", scene.name(), -5, 0);
        }

    private:
        sf::Font _font;
        sf::RenderWindow _window;

        uint32_t _ticks;
        const uint32_t _jumps;
        std::chrono::_V2::system_clock::time_point _lastUpdate;
        std::vector<double> _tickUpdates = {};
    };
}
