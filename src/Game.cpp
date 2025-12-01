#include "../include/Game.hpp"

Game::Game(unsigned int windowWidth, unsigned int windowHeight) {
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Coin Runner Game");

    tempPlayer.setSize({50.f, 50.f});
    tempPlayer.setFillColor(sf::Color::Green);
    tempPlayer.setPosition({windowWidth / 2.f, windowHeight / 2.f});
    tempPlayer.setOrigin(tempPlayer.getSize() / 2.f);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (event->is<sf::Event::Resized>()) {
            sf::View view(
                {window.getSize().x / 2.f, window.getSize().y / 2.f},
                {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)}
            );
            window.setView(view);
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                window.close();
            }
        }
    }
}

void Game::update(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        tempPlayer.move({0.f, -speed * dt});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        tempPlayer.move({0.f, speed * dt});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        tempPlayer.move({-speed * dt, 0.f});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        tempPlayer.move({speed * dt, 0.f});
}

void Game::render() {
    window.clear(sf::Color::Blue);
    window.draw(tempPlayer);
    window.display();
}