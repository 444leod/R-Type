/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "Foo.hpp"
#include <asio.hpp> // found it
#include <SFML/Graphics.hpp> // found it

int main(void)
{
    std::cout << "Client said o:" << std::endl;
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    Foo::say();
    return 0;
}
