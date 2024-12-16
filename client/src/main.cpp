#include <thread>
#include <memory>
#include "Client.hpp"

int main(void)
{
    std::unique_ptr<std::thread> t;
    try
    {
        asio::io_context ctx;
        Client client(ctx);
        t = std::make_unique<std::thread>([&ctx](){
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
        t->join();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        if (t && t->joinable()) {
            t->join();
        }
        return 1;
    }

    return 0;
}