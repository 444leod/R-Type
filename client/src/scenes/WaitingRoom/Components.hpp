/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Components.hpp
*/

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <functional>

struct Position { float x, y; };
struct Renderable { sf::Text text; };
struct Button { sf::RectangleShape shape; std::string label; std::function<void()> onClick; };

struct Debug {};

#endif //COMPONENTS_HPP
