#include <SFML/Graphics.hpp>
#include "TileMap.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1280u, 720u}),
        "Map Loader Test",
        sf::State::Windowed
    );

    window.setFramerateLimit(60);

    TileMap map;
    if (!map.loadFromFile("asset/maps/mapdemo.json", 16))
        return -1;

    sf::View view(
        sf::Vector2f(640.f, 360.f),
        sf::Vector2f(1280.f, 720.f)
    );

    while (window.isOpen())
    {
        while (auto evt = window.pollEvent())
        {
            // 🔥 KIỂM TRA EVENT ĐÓNG CỬA
            if (evt->getIf<sf::Event::Closed>())
                window.close();
        }

        // di chuyển camera
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
            view.move({5, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
            view.move({-5, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down))
            view.move({0, 5});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
            view.move({0, -5});

        window.setView(view);

        window.clear();
        window.draw(map);
        window.display();
    }

    return 0;
}