/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include <memory>
#include <thread>

#include "Client.hpp"
#include "SceneManager.hpp"
#include "scenes/Level1/Level1.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"

int main(void) {
    asio::io_context ctx;
    std::unique_ptr<std::thread> t;
    try {
        SceneManager sceneManager(ctx);
        t = std::make_unique<std::thread>([&ctx]() { ctx.run(); });
        sceneManager.registerScene<WaitingRoom>("WaitingRoom");
        sceneManager.registerScene<Level1>("Level1");
        sceneManager.load("WaitingRoom");
        sceneManager.run();
        std::cout << "Game ended..." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        if (t && t->joinable()) {
            t->join();
        }
        return 1;
    }
    if (t && t->joinable()) {
        t->join();
    }
    return 0;
}
