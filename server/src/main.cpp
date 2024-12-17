/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <thread>
#include "Server.hpp"

int main(void)
{
    asio::io_context ctx;
    std::unique_ptr<std::thread> t;
    try
    {
        Server server(ctx);
        t = std::make_unique<std::thread>([&ctx](){
            ctx.run();
        });
        server.run();
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
