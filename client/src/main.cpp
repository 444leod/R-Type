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

void create_projectile(Registry& registry, Position pos) {
    const auto projectile = registry.create();

    static sf::Texture tex = sf::Texture();
    static bool set = false;
    if (!set) {
        tex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(100, 170, 85, 16));
        set = true;
    }
    static auto sprite = Sprite{.sprite = sf::Sprite(tex)};
    sprite.sprite.setOrigin(85 / 2, 16 / 2);

    registry.addComponent<std::shared_ptr<Sprite>>(projectile, std::make_shared<Sprite>(sprite));
    registry.addComponent<std::shared_ptr<Position>>(projectile, std::make_shared<Position>(pos));
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
    auto sprite = Sprite{.sprite = sf::Sprite(tex)};
    sprite.sprite.setScale(3, 3);
    sprite.sprite.setOrigin(34 / 2, 18 / 2);

    registry.addComponent<std::shared_ptr<Sprite>>(spaceship, std::make_shared<Sprite>(sprite));
    auto position = Position{.x = 100, .y = 100};
    registry.addComponent<std::shared_ptr<Position>>(spaceship, std::make_shared<Position>(position));

    registry.addComponent<Self>(spaceship, Self{});
    registry.addComponent<Hitbox>(spaceship, Hitbox{});

    auto self_view = registry.view<Self, std::shared_ptr<Position>>().get();
    assert(self_view.size() == 1 && "self view size is not 1");
    auto [self_entity, self_tuple] = self_view[0];

    std::vector<sf::Keyboard::Key> pressed_keys;

    sf::Clock clock;
    const sf::Time frameTime = sf::seconds(1.f / 60.f);

    while (running) {
        sf::Time elapsed = clock.restart();

        auto view = registry.view<std::shared_ptr<Sprite>, std::shared_ptr<Position>>().get();
        for (auto &[entity, tuple] : view) {
            auto sp = std::get<std::shared_ptr<Sprite>>(tuple);
            auto pos = std::get<std::shared_ptr<Position>>(tuple);
            sp->sprite.setPosition(pos->x, pos->y);
            window.draw(sp->sprite);
        }

        auto hitbox_view = registry.view<Hitbox, std::shared_ptr<Sprite>>().get();
        for (auto &[entity, tuple] : hitbox_view) {
            auto sp = std::get<std::shared_ptr<Sprite>>(tuple);
            auto rect = sp->sprite.getGlobalBounds();
            sf::RectangleShape shape(sf::Vector2f(rect.width, rect.height));
            shape.setFillColor(sf::Color::Transparent);
            shape.setPosition(rect.left, rect.top);
            shape.setOutlineColor(sf::Color::Red);
            shape.setOutlineThickness(1);
            window.draw(shape);
        }



        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    create_projectile(registry, *std::get<std::shared_ptr<Position>>(self_tuple));
                }
                pressed_keys.push_back(event.key.code);
            } else if (event.type == sf::Event::KeyReleased) {
                pressed_keys.erase(std::remove(pressed_keys.begin(), pressed_keys.end(), event.key.code), pressed_keys.end());
            }
        }
        for (auto key : pressed_keys) {
            switch (key) {
                case sf::Keyboard::Key::Right:
                    std::get<std::shared_ptr<Position>>(self_tuple)->x += 5;
                    break;
                case sf::Keyboard::Key::Left:
                    std::get<std::shared_ptr<Position>>(self_tuple)->x -= 5;
                    break;
                case sf::Keyboard::Key::Up:
                    std::get<std::shared_ptr<Position>>(self_tuple)->y -= 5;
                    break;
                case sf::Keyboard::Key::Down:
                    std::get<std::shared_ptr<Position>>(self_tuple)->y += 5;
                    break;
                default:
                    break;
            }
        }

        auto projectile_view = registry.view<std::shared_ptr<Position>, Projectile, Velocity>().get();
        for (auto &[entity, tuple] : projectile_view) {
            auto pos = std::get<std::shared_ptr<Position>>(tuple);
            auto vel = std::get<Velocity>(tuple);
            pos->x += vel.velocity.x;
            pos->y += vel.velocity.y;

            if (pos->x > 1200) {
                registry.remove(entity);
            }
        }
        window.display();
        window.clear(sf::Color::Black);

        if (elapsed < frameTime) {
            sf::sleep(frameTime - elapsed);
        }
    }
    std::cout << "Client said:" << std::endl;
    Foo::say();
    return 0;
}
