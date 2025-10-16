// src/main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // VideoMode nhận sf::Vector2u (dùng suffix 'u' để rõ là unsigned)
    sf::VideoMode vm({800u, 600u});
    sf::RenderWindow window(vm, "CoinRunner Test");

    window.setFramerateLimit(60);

    std::cout << "SFML 3.0.2 window created successfully!" << std::endl;

    while (window.isOpen()) {
        // pollEvent() trả về std::optional<sf::Event>
        while (auto event = window.pollEvent()) {
            // Nếu là sự kiện đóng cửa sổ
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Ví dụ: nhấn Escape để đóng luôn
            if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();
                if (key && key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color(100, 150, 255)); // nền xanh
        window.display();
    }

    return 0;
}
