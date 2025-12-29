#pragma once
#include <SFML/Graphics.hpp>

namespace UITheme
{
    // ---------- FONT ----------
    inline constexpr unsigned int FONT_SIZE = 12;

    // ---------- TEXT SCALE ----------
    inline constexpr float TEXT_SCALE_NORMAL = 2.f;

    // ---------- BUTTON SCALE ----------
    inline constexpr sf::Vector2f BTN_SCALE_NORMAL { 12.f, 4.f };

    // ---------- COLORS ----------
    inline constexpr sf::Color TEXT_NORMAL  { 82, 54, 32 };
    inline constexpr sf::Color TEXT_HOVER   { 255, 196, 70 };
    inline constexpr sf::Color TEXT_PRESSED { 255, 170, 40 };
    inline constexpr sf::Color TEXT_LOCKED  { 150, 150, 150 };

    inline constexpr sf::Color TEXT_SHADOW  { 0, 0, 0, 120 };
    inline constexpr sf::Color OVERLAY_COLOR{ 0, 0, 0, 30 };

    // ---------- SHADOW ----------
    inline constexpr sf::Vector2f SHADOW_OFFSET { 2.f, 2.f };

    // ---------- HELPERS ----------
    inline void drawOverlay(sf::RenderWindow& window)
    {
        sf::RectangleShape overlay;
        overlay.setSize({
            (float)window.getSize().x,
            (float)window.getSize().y
        });
        overlay.setFillColor(OVERLAY_COLOR);
        window.draw(overlay);
    }

    inline void drawTextWithShadow(sf::RenderWindow& window, const sf::Text& text)
    {
        sf::Text shadow = text;
        shadow.move(SHADOW_OFFSET);
        shadow.setFillColor(TEXT_SHADOW);
        window.draw(shadow);
        window.draw(text);
    }
}