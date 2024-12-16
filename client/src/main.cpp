/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/


#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"
#include "scenes/Level1/Level1.hpp"

int main(void) {
    SceneManager sceneManager;

    sceneManager.registerScene<Level1>("Game");

    sceneManager.load("Game");
    sceneManager.run();
}
