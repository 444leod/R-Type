/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <thread>
#include "Client.hpp"

int main(void)
{
    asio::io_context ctx;
    Client client(ctx);
    std::thread t([&ctx](){
        ctx.run();
    });
    client.run("127.0.0.1", 25565);

    client.sendMessage("Hello boop boop zib zib");

    UDPPacket packet;
    packet << PACKET_TYPE::POSITION;
    packet << Position{10.0f, 20.0f};
    client.send(packet);

    while (client.running())
    {
        sf::Event e;
        while (client.window().pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                client.stop();
        }
        client.window().clear();
        client.window().display();
    }
    t.join();
    return 0;
}
