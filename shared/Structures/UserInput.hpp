/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UserInput.hpp
*/

#ifndef USERINPUT_HPP
#define USERINPUT_HPP

#include <SFML/Window/Keyboard.hpp>

struct UserInput {
    sf::Keyboard::Key key;
    bool pressed;
};

#endif // USERINPUT_HPP
