/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <thread>
// #include "Server.hpp"
#include "SceneManager.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"
#include "scenes/Level1/Level1.hpp"

int main(void)
{
    try
    {
        SceneManager sceneManager(25565);
        sceneManager.registerScene<WaitingRoom>("WaitingRoom");
        sceneManager.registerScene<Level1>("Level1");
        sceneManager.load("WaitingRoom");
        sceneManager.run();
        std::cout << "Game ended..." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
