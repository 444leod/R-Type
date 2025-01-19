/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** CollisionSystem.hpp
*/

#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <Engine/Systems/AUpdateSystem.hpp>

#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Transform.hpp"

#include <cmath>
#include <vector>

inline bool pointInRectangle(const float& px, const float& py, const float& rx, const float& ry, const float& rw, const float& rh, const float& cosAngle, const float& sinAngle)
{
    const float localX = px - rx;
    const float localY = py - ry;

    const float rotatedX = localX * cosAngle + localY * sinAngle;
    const float rotatedY = -localX * sinAngle + localY * cosAngle;

    return std::abs(rotatedX) <= rw / 2 && std::abs(rotatedY) <= rh / 2;
}

class CollisionSystem final : public engine::AUpdateSystem
{
  public:
    explicit CollisionSystem() : AUpdateSystem("CollisionSystem") {}

    void execute(const double&) override
    {
        auto view = _registry.view<Hitbox, Transform>();
        std::vector<std::pair<ecs::Entity, ecs::Entity>> collisions;

        for (auto it1 = view.begin(); it1 != view.end(); ++it1)
        {
            auto [entity1, hitbox1, transform1] = *it1;

            for (auto it2 = it1; ++it2 != view.end();)
            {
                auto [entity2, hitbox2, transform2] = *it2;

                if (checkCollision(hitbox1, transform1, hitbox2, transform2))
                {
                    collisions.emplace_back(entity1, entity2);
                }
            }
        }

        for (const auto& [entity1, entity2] : collisions)
        {
            if (_registry.has_any_of<Hitbox>(entity1)) {
                auto& hitbox1 = _registry.get<Hitbox>(entity1);
                hitbox1.onCollision(entity2);
            }
            if (_registry.has_any_of<Hitbox>(entity2)) {
                auto& hitbox2 = _registry.get<Hitbox>(entity2);
                hitbox2.onCollision(entity1);
            }
        }
    }

  private:
    static bool checkCollision(const Hitbox& hitbox1, const Transform& transform1, const Hitbox& hitbox2, const Transform& transform2)
    {
        if (std::holds_alternative<shape::Rectangle>(hitbox1.shape) && std::holds_alternative<shape::Rectangle>(hitbox2.shape))
        {
            return checkRectangleCollision(std::get<shape::Rectangle>(hitbox1.shape), transform1, std::get<shape::Rectangle>(hitbox2.shape), transform2);
        }
        if (std::holds_alternative<shape::Circle>(hitbox1.shape) && std::holds_alternative<shape::Circle>(hitbox2.shape))
        {
            return checkCircleCollision(std::get<shape::Circle>(hitbox1.shape), transform1, std::get<shape::Circle>(hitbox2.shape), transform2);
        }
        if (std::holds_alternative<shape::Circle>(hitbox1.shape) && std::holds_alternative<shape::Rectangle>(hitbox2.shape))
        {
            return checkCircleRectangleCollision(std::get<shape::Circle>(hitbox1.shape), transform1, std::get<shape::Rectangle>(hitbox2.shape), transform2);
        }
        if (std::holds_alternative<shape::Rectangle>(hitbox1.shape) && std::holds_alternative<shape::Circle>(hitbox2.shape))
        {
            return checkCircleRectangleCollision(std::get<shape::Circle>(hitbox2.shape), transform2, std::get<shape::Rectangle>(hitbox1.shape), transform1);
        }
        return false;
    }

    static bool checkRectangleCollision(const shape::Rectangle& rect1, const Transform& transform1, const shape::Rectangle& rect2, const Transform& transform2)
    {
        if (transform1.rotation == 0 && transform2.rotation == 0)
        {
            return (transform1.x < transform2.x + rect2.width && transform1.x + rect1.width > transform2.x && transform1.y < transform2.y + rect2.height && transform1.y + rect1.height > transform2.y);
        }

        const double angle1 = transform1.rotation * (M_PI / 180.0f);
        const double angle2 = transform2.rotation * (M_PI / 180.0f);

        const double cosAngle1 = std::cos(angle1);
        const double sinAngle1 = std::sin(angle1);
        const double cosAngle2 = std::cos(angle2);
        const double sinAngle2 = std::sin(angle2);

        auto rotatePoint = [](const double& x, const double& y, const double& cosAngle, const double& sinAngle) { return std::make_pair(x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle); };

        auto [rect1TopLeftX, rect1TopLeftY] = rotatePoint(-rect1.width / 2, -rect1.height / 2, cosAngle1, sinAngle1);
        auto [rect1TopRightX, rect1TopRightY] = rotatePoint(rect1.width / 2, -rect1.height / 2, cosAngle1, sinAngle1);
        auto [rect1BottomLeftX, rect1BottomLeftY] = rotatePoint(-rect1.width / 2, rect1.height / 2, cosAngle1, sinAngle1);
        auto [rect1BottomRightX, rect1BottomRightY] = rotatePoint(rect1.width / 2, rect1.height / 2, cosAngle1, sinAngle1);

        auto [rect2TopLeftX, rect2TopLeftY] = rotatePoint(-rect2.width / 2, -rect2.height / 2, cosAngle2, sinAngle2);
        auto [rect2TopRightX, rect2TopRightY] = rotatePoint(rect2.width / 2, -rect2.height / 2, cosAngle2, sinAngle2);
        auto [rect2BottomLeftX, rect2BottomLeftY] = rotatePoint(-rect2.width / 2, rect2.height / 2, cosAngle2, sinAngle2);
        auto [rect2BottomRightX, rect2BottomRightY] = rotatePoint(rect2.width / 2, rect2.height / 2, cosAngle2, sinAngle2);

        if (pointInRectangle(transform1.x + rect1TopLeftX, transform1.y + rect1TopLeftY, transform2.x, transform2.y, rect2.width, rect2.height, cosAngle2, sinAngle2) ||
            pointInRectangle(transform1.x + rect1TopRightX, transform1.y + rect1TopRightY, transform2.x, transform2.y, rect2.width, rect2.height, cosAngle2, sinAngle2) ||
            pointInRectangle(transform1.x + rect1BottomLeftX, transform1.y + rect1BottomLeftY, transform2.x, transform2.y, rect2.width, rect2.height, cosAngle2, sinAngle2) ||
            pointInRectangle(transform1.x + rect1BottomRightX, transform1.y + rect1BottomRightY, transform2.x, transform2.y, rect2.width, rect2.height, cosAngle2, sinAngle2))
        {
            return true;
        }

        if (pointInRectangle(transform2.x + rect2TopLeftX, transform2.y + rect2TopLeftY, transform1.x, transform1.y, rect1.width, rect1.height, cosAngle1, sinAngle1) ||
            pointInRectangle(transform2.x + rect2TopRightX, transform2.y + rect2TopRightY, transform1.x, transform1.y, rect1.width, rect1.height, cosAngle1, sinAngle1) ||
            pointInRectangle(transform2.x + rect2BottomLeftX, transform2.y + rect2BottomLeftY, transform1.x, transform1.y, rect1.width, rect1.height, cosAngle1, sinAngle1) ||
            pointInRectangle(transform2.x + rect2BottomRightX, transform2.y + rect2BottomRightY, transform1.x, transform1.y, rect1.width, rect1.height, cosAngle1, sinAngle1))
        {
            return true;
        }

        return false;
    }

    static bool checkCircleCollision(const shape::Circle& circle1, const Transform& transform1, const shape::Circle& circle2, const Transform& transform2)
    {
        const double dx = transform1.x - transform2.x;
        const double dy = transform1.y - transform2.y;
        const double distance = std::sqrt(dx * dx + dy * dy);
        return distance < (circle1.radius + circle2.radius);
    }

    static bool checkCircleRectangleCollision(const shape::Circle& circle, const Transform& circleTransform, const shape::Rectangle& rect, const Transform& rectTransform)
    {
        const double angle = rectTransform.rotation * (M_PI / 180.0f);
        const double cosAngle = std::cos(angle);
        const double sinAngle = std::sin(angle);

        const double rectCenterX = rectTransform.x + rect.width / 2;
        const double rectCenterY = rectTransform.y + rect.height / 2;

        const double localCircleX = circleTransform.x - rectCenterX;
        const double localCircleY = circleTransform.y - rectCenterY;

        const double rotatedCircleX = localCircleX * cosAngle + localCircleY * sinAngle;
        const double rotatedCircleY = -localCircleX * sinAngle + localCircleY * cosAngle;

        const double circleDistanceX = std::abs(rotatedCircleX);
        const double circleDistanceY = std::abs(rotatedCircleY);

        if (circleDistanceX > (rect.width / 2 + circle.radius))
        {
            return false;
        }
        if (circleDistanceY > (rect.height / 2 + circle.radius))
        {
            return false;
        }

        if (circleDistanceX <= (rect.width / 2))
        {
            return true;
        }
        if (circleDistanceY <= (rect.height / 2))
        {
            return true;
        }

        const double cornerDistance_sq = std::pow(circleDistanceX - rect.width / 2, 2) + std::pow(circleDistanceY - rect.height / 2, 2);

        return (cornerDistance_sq <= std::pow(circle.radius, 2));
    }
};

#endif // COLLISION_SYSTEM_HPP
