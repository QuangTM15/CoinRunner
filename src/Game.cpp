#include "../include/Game.hpp"
#include <SFML/Window.hpp>
#include <iostream>

Game::Game(unsigned int windowWidth, unsigned int windowHeight) {
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Coin Runner Game");

    // camera follow player
    camera.setCenter({windowWidth / 2.f, windowHeight / 2.f});
    camera.setSize({static_cast<float>(windowWidth), static_cast<float>(windowHeight)});
    window.setView(camera);

    tileMap.loadDemoMap();
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
            camera = view;
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                window.close();
            }
        }
    }
}

void Game::update(float dt) {
    // cập nhật player
    player.update(dt);
    // cập nhật timer trap cooldown
    player.updateTrapTimer(dt);

    // collision với tile map
    sf::Vector2f correction = tileMap.checkCollision(player.getBounds(), player.getVelocity());
    player.correctPosition(correction);

    // Lấy bounds Player
    sf::Rect<float> playerBox = player.getBounds();

    // Rút viền để tránh chạm trap/coin quá sớm
    playerBox.position.x   += 5.f;
    playerBox.position.y   += 5.f;
    playerBox.size.x       -= 10.f;
    playerBox.size.y       -= 10.f;

    // kiểm tra coin
    for (auto it = tileMap.coins.begin(); it != tileMap.coins.end();) {
        if (it->checkCollected(player.getBounds())) {

            coinCount++;
            std::cout << "Collected a coin! Total: " << coinCount << std::endl;
            it = tileMap.coins.erase(it);
        } else {
            ++it;
        }
    }

    // kiểm tra trap
    for (auto& trap : tileMap.traps) {
        if (trap.checkHit(player.getBounds())) {
            player.hitTrap();
        }
        else {
            player.resetHitTrap();
        }
    }

    // camera follow player
    sf::Vector2f pos = player.getPosition();
    camera.setCenter(pos);
    window.setView(camera);
}

void Game::render() {
    window.clear(sf::Color::Blue);

    tileMap.draw(window);  // draw ground, coin, trap
    player.draw(window);

    window.display();
}