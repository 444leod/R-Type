/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/


#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"
#include "scenes/Game/Game.hpp"

int main(void) {
    SceneManager sceneManager;

    sceneManager.registerScene<Game>("Game");

    sceneManager.load("Game");
    sceneManager.run();
}
