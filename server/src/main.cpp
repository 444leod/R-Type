/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <thread>
#include "Game.hpp"
#include "SceneManager.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"
#include "scenes/Level1/Level1.hpp"

int main(void)
{
    auto game = Game();

    game.scenes().registerScene<WaitingRoom>("main");
    game.scenes().registerScene<Level1>("game");
    game.run();
    return 0;
}
