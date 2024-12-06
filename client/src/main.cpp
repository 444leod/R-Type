/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "Foo.hpp"
#include "Registry.hpp"
#include <SFML/Graphics.hpp>

struct Sprite {
    sf::Sprite sprite;
};

struct Position {
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Self{};

struct Velocity {
    sf::Vector2f velocity;
};

struct Projectile {
};

struct Hitbox {};

void create_projectile(Registry& registry, const Position& pos) {
    const auto projectile = registry.create();

    static auto  tex = sf::Texture();
    static bool set = false;
    if (!set) {
        tex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(100, 170, 85, 16));
        set = true;
    }
    static auto sprite = sf::Sprite(tex);
    sprite.setOrigin(85 / 2, 16 / 2);

    registry.addComponent<sf::Sprite>(projectile, sprite);
    registry.addComponent<Position>(projectile, pos);
    registry.addComponent<Projectile>(projectile, Projectile{});
    registry.addComponent<Velocity>(projectile, Velocity{.velocity = sf::Vector2f(10, 0)});
    registry.addComponent<Hitbox>(projectile, Hitbox{});
}

int main(void)
{
    sf::VideoMode mode;
    mode.width =1600;
    mode.height = 800;
    mode.bitsPerPixel =32;

    sf::RenderWindow window;
    window.create(mode, "R-Type", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(false);

    sf::Event event{};
    bool running = true;

    Registry registry;

    auto spaceship = registry.create();

    auto tex = sf::Texture();
    assert(tex.loadFromFile("assets/r-typesheet42.gif", sf::IntRect(0, 0, 34, 18)) && "sprite doesnt exist");
    auto sprite = sf::Sprite(tex);
    sprite.setScale(3, 3);
    sprite.setOrigin(34 / 2, 18 / 2);

    registry.addComponent(spaceship, sprite);
    auto position = Position{.x = 100, .y = 100};
    registry.addComponent<Position>(spaceship, position);

    registry.addComponent<Self>(spaceship, Self{});
    registry.addComponent<Hitbox>(spaceship, Hitbox{});

    std::vector<sf::Keyboard::Key> pressed_keys;

    sf::Clock clock;
    const sf::Time frameTime = sf::seconds(1.f / 60.f);

    while (running) {
        window.clear(sf::Color::Black);

        sf::Time elapsed = clock.restart();

        auto view = registry.view<sf::Sprite, Position>();

        for (auto [entity, sp, pos] : view.each() ) {
            sp.setPosition(pos.x, pos.y);
            window.draw(sp);
        }

        // for (auto [entity, sp, pos] : view) {
        //     sp.setPosition(pos.x, pos.y);
        //     window.draw(sp);
        // }

        /*
        for (auto entity : view.entities()) {
            auto& sp = view.get<sf::Sprite>(entity);
            const auto& pos = view.get<Position>(entity);

            // auto& random_pos = view.get<Position>(876329);  will throw an exception
            // auto& vel = view.get<Velocity>(entity);  will not compile

            sp.setPosition(pos.x, pos.y);
            window.draw(sp);
        }
        */

        // view.each([&](sf::Sprite& sp, const Position& pos) {
        //     sp.setPosition(pos.x, pos.y);
        //     window.draw(sp);
        // });

        registry.view<Hitbox, sf::Sprite>().each([&](const Hitbox&, const sf::Sprite& sp) {
            const auto rect = sp.getGlobalBounds();
            sf::RectangleShape shape(sf::Vector2f(rect.width, rect.height));
            shape.setFillColor(sf::Color::Transparent);
            shape.setPosition(rect.left, rect.top);
            shape.setOutlineColor(sf::Color::Red);
            shape.setOutlineThickness(1);
            window.draw(shape);
        });

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    registry.view<Self, Position>().each([&](const Self&, const Position& pos) {
                        create_projectile(registry, pos);
                    });
                }
                pressed_keys.push_back(event.key.code);
            } else if (event.type == sf::Event::KeyReleased) {
                std::erase(pressed_keys, event.key.code);
            }
        }
        for (auto key : pressed_keys) {
            switch (key) {
                case sf::Keyboard::Key::Right:
                    registry.view<Self, Position>().each([&](const Self&, Position& pos) {
                        pos.x += 5;
                    });
                    break;
                case sf::Keyboard::Key::Left:
                    registry.view<Self, Position>().each([&](const Self&, Position& pos) {
                        pos.x -= 5;
                    });
                    break;
                case sf::Keyboard::Key::Up:
                    registry.view<Self, Position>().each([&](const Self&, Position& pos) {
                        pos.y -= 5;
                    });
                    break;
                case sf::Keyboard::Key::Down:
                    registry.view<Self, Position>().each([&](const Self&, Position& pos) {
                        pos.y += 5;
                    });
                    break;
                default:
                    break;
            }
        }

        registry.view<Position, Projectile, Velocity>().each([&](const std::size_t& entity, Position& pos, const Projectile&, const Velocity& vel) {
            pos.x += vel.velocity.x;
            pos.y += vel.velocity.y;

            if (pos.x > 1200) {
                registry.remove(entity);
            }
        });

        window.display();

        if (elapsed < frameTime) {
            sf::sleep(frameTime - elapsed);
        }
    }
    std::cout << "Client said:" << std::endl;
    Foo::say();
    return 0;
}
