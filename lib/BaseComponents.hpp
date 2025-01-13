/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BaseComposants
*/

#ifndef BASE_COMPONENTS_HPP_
#define BASE_COMPONENTS_HPP_

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <utility>
#include <filesystem>
#include <bits/fs_ops.h>
#include <iostream>

#include "Config.hpp"

// TODO: Decide how to organize the components in the lib

struct Transform {
    float x, y, z;
    float rotation;
};

struct Velocity {
    float x, y;
};

struct Self {};

struct Hitbox {};

struct Projectile {
    double range;
    std::uint32_t id;
};

struct Parallax {
    int offsetMultiplier = 0;
    float offset = 0;
};

struct Position {
    float x, y;
};

struct Animation {
    sf::Clock clock = sf::Clock();
    std::pair<int, int> frameSize{};
    float speed{};
    unsigned int currentFrame = 1;
    unsigned int frameCount{};
    bool loop{};
    // TODO: Refacto to not have nested components
    Velocity velocity{};
};

struct Enemy {
    std::uint32_t id;
};

// UI Components

struct Text {
    std::string message;
    std::string font;
    std::uint32_t fontSize;
    struct {
        std::uint32_t r;
        std::uint32_t g;
        std::uint32_t b;
    } color;
};

class DrawSpritesSystem;

struct Sprite
{
private:
    std::uint32_t pathId;
    static std::unordered_map<std::string, std::uint32_t> loadedTextures;
    friend class DrawSpritesSystem;

public:
    std::string texture;
    std::pair<float, float> scale;
    std::pair<float, float> origin;
    std::optional<sf::IntRect> textureRect;

public:
    explicit Sprite(
        std::string texture,
        const std::pair<double, double>& scale = {1.0, 1.0},
        const std::pair<double, double>& origin = {0.0, 0.0},
        const std::optional<sf::IntRect>& textureRect = std::nullopt)
        : texture(std::move(texture)), scale({ scale.first * SCALE, scale.second * SCALE }), origin(origin), textureRect(textureRect)
    {
        const auto path = std::filesystem::weakly_canonical(this->texture).string();

        if (auto it = loadedTextures.find(path); it == loadedTextures.end())
        {
            pathId = loadedTextures.size();
            loadedTextures[path] = pathId;
        }
        else
        {
            pathId = it->second;
        }
    }
};

inline std::unordered_map<std::string, std::uint32_t> Sprite::loadedTextures;

struct RectangleButton {
    sf::RectangleShape shape;
    std::string label;
    std::function<void()> onClick;
};

#endif /* !BASECOMPONENTS_HPP_ */
