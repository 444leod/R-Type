/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "Game.hpp"
#include "scenes/Level1/Level1.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"

int main(void) {
    auto game = Game();

    game.scenes().registerScene<WaitingRoom>("main");
    game.scenes().registerScene<Level1>("game");
    game.run();
    return 0;
}
