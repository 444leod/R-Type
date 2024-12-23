/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <thread>
#include "engine/Game.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"
#include "scenes/Level1/Level1.hpp"

int main(void)
{
    auto game = Game();

    game.registerScene<WaitingRoom>("main");
    game.registerScene<Level1>("game");
    game.run();
    return 0;
}
