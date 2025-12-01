#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Game {
    public:
        Game(unsigned int windowWidth, unsigned int windowHeight);
        void run();

    private:
        void processEvents();
        void update(float dt);
        void render();

    private:
        sf::RenderWindow window;
        sf::Clock clock;

        //temp player shape:
        sf::RectangleShape tempPlayer;
        float speed=200.f;
};

