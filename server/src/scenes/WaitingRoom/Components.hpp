/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Components.hpp
*/

#ifndef WAITING_ROOM_COMPONENTS_HPP
#define WAITING_ROOM_COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <functional>

struct Position { float x, y; };
struct Renderable { sf::Text text; };
struct Button { sf::RectangleShape shape; std::string label; std::function<void()> onClick; };

struct Debug {};

#endif //WAITING_ROOM_COMPONENTS_HPP
