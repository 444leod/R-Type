/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "scenes/Game/Game.hpp"
#include <Engine/Game.hpp>

#include "PremadeModules/Rendering/GameRenderingModule.hpp"
#include "PremadeModules/Rendering/ASceneRenderingModule.hpp"

#include "scenes/HomeScreen/HomeScreen.hpp"

int main() {
    auto game = engine::Game();

    game.addModule<GameRenderingModule>(598, 1012, "Orchera");

    const auto homeScreen = game.registerScene<HomeScreen>("home screen");
    {
        homeScreen->addModule<ASceneRenderingModule>();
    }


    const auto gameScene = game.registerScene<Game>("game");
    {
        gameScene->addModule<ASceneRenderingModule>();
    }

    game.run();
    return 0;
}
