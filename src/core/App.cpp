#include "core/App.hpp"
#include "scene/PlayScene.hpp"
#include "scene/MainMenuScene.hpp"
#include <iostream>

App::App()
: window(sf::VideoMode({1280u, 720u}), "CoinRunner")
, sceneManager()
{
    window.setFramerateLimit(60);

    sceneManager.change(
        std::make_unique<MainMenuScene>(sceneManager, window)
    );
}

void App::run()
{
    std::cout << "[App] run()\n";

    while (window.isOpen())
    {
        processEvents();

        float dt = clock.restart().asSeconds();
        dt = std::min(dt, 1.f / 60.f);
        update(dt);
        render();
    }
}

void App::processEvents()
{
    while (auto evt = window.pollEvent())
    {
        if (evt->is<sf::Event::Closed>())
            window.close();

        sceneManager.handleEvent(*evt);
    }
}

void App::update(float dt)
{
    sceneManager.update(dt);
}

void App::render()
{
    window.clear(sf::Color::Black);

    // GAME WORLD
    sceneManager.render(window);

    // 🔥 RESET VIEW cho UI
    window.setView(window.getDefaultView());

    // (sau này vẽ HUD ở đây)

    window.display();
}
