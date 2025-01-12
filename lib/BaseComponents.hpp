/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BaseComposants
*/

#ifndef BASE_COMPONENTS_HPP_
#define BASE_COMPONENTS_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <filesystem>
#include <bits/fs_ops.h>
#include <variant>
#include <optional>

#include "ecs/Entity.hpp"
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

struct Color
{
    std::int8_t r;
    std::int8_t g;
    std::int8_t b;
    std::int8_t a = static_cast<std::int8_t>(255);
};

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
    Color color;
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

namespace shape
{
struct Circle
{
    float radius;
    Color fillColor;
    Color outlineColor;
    float outlineThickness;
};

struct Rectangle
{
    float width;
    float height;
    Color fillColor = {0, 0, 0};
    Color outlineColor = {0, 0, 0};
    float outlineThickness = 0;
    float rotation = 0;
};
}

struct Hitbox
{
    std::variant<shape::Rectangle, shape::Circle> shape;
    std::function<void(const Entity& other)> onCollision;
};

struct Debug {};

#endif /* !BASECOMPONENTS_HPP_ */
