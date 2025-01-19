/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ASceneAudioModule
*/

#pragma once

#include <SFML/Audio.hpp>
#include "Engine/Modules/ASceneModule.hpp"
#include "Engine/AScene.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <memory>


class ASceneAudioModule final : public engine::ASceneModule
{
public:
    explicit ASceneAudioModule(engine::AScene& scene) : engine::ASceneModule(scene) {}
    ~ASceneAudioModule() override = default;

    /**
     * @brief Play a music
     * @param path Path to the music file
     * @param loop Should the music loop
     */
    void playMusic(const std::string& path, bool loop = false)
    {
        _music = std::make_unique<sf::Music>();
        if (!_music->openFromFile(path)) {
            std::cerr << "Failed to stream music of path: " << path << std::endl;
            _music.reset();
            return;
        }
        _music->setLoop(loop);
        _music->play();
    }

    /**
     * @brief Load a sound
     * @param path Path to the sound file
     * @return true if the sound was loaded successfully, false otherwise
     */
    bool loadSound(const std::string& path)
    {
        if (_soundBuffers.find(path) != _soundBuffers.end())
            return true;

        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(path)) {
            std::cerr << "Failed to load sound of path: " << path << std::endl;
            return false;
        }
        _soundBuffers[path] = std::make_shared<sf::SoundBuffer>(std::move(buffer));
        return true;
    }

    /**
     * @brief Play a sound
     * @param path Path to the sound file
     */
    void playSound(const std::string& path)
    {
        if (_soundBuffers.find(path) == _soundBuffers.end() && !loadSound(path)) {
            std::cerr << "Failed to play sound of path: " << path << std::endl;
            return;
        }

        auto sound = std::make_unique<sf::Sound>();
        sound->setBuffer(*_soundBuffers[path]);
        sound->play();
        _activeSounds.push_back(std::move(sound));
    }

    /**
     * @brief Update active sounds to remove those that have stopped playing
     */
    void update()
    {
        _activeSounds.erase(std::remove_if(
            _activeSounds.begin(),
            _activeSounds.end(),
            [](const std::unique_ptr<sf::Sound>& sound) { return sound->getStatus() == sf::Sound::Stopped; }),
            _activeSounds.end());
    }

    void stop()
    {
        _music.reset();
        _soundBuffers.clear();
        _activeSounds.clear();
    }

private:
    std::unique_ptr<sf::Music> _music;
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> _soundBuffers;
    std::vector<std::unique_ptr<sf::Sound>> _activeSounds;
};