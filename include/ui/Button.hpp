#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>

// ================= BUTTON STATE =================
enum class ButtonState {
    Normal,
    Hover,
    Pressed,
    Locked
};

// ================= BUTTON =================
struct Button
{
    // UI
    std::optional<sf::Text>   text;
    std::optional<sf::Sprite> sprite;

    // Textures
    sf::Texture texNormal;
    sf::Texture texHover;
    sf::Texture texPressed;

    // State
    ButtonState state = ButtonState::Normal;

    // Action
    std::function<void()> action;
};