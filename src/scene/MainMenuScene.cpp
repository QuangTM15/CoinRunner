#include "scene/MainMenuScene.hpp"
#include <iostream>

MainMenuScene::MainMenuScene(SceneManager& mgr, sf::RenderWindow& win)
: Scene(mgr)
, window(win)
{
    initAssets();
    initMenuItems();
    initLayout();
}

void MainMenuScene::onEnter()
{
    std::cout << "[MainMenuScene] onEnter\n";
}

void MainMenuScene::onExit()
{
    std::cout << "[MainMenuScene] onExit\n";
}

void MainMenuScene::handleEvent(const sf::Event& e)
{
    if (e.is<sf::Event::KeyPressed>())
    {
        handleKeyboard(e.getIf<sf::Event::KeyPressed>()->code);
    }
}

void MainMenuScene::update(float)
{
}

void MainMenuScene::render(sf::RenderWindow& window)
{
    static bool once = false;
    if (!once)
    {
        std::cout << "[MainMenuScene] render()\n";
        once = true;
    }
    if (panelSprite)
        window.draw(*panelSprite);

    for (auto& item : items)
    {
        if (item.sprite)
            window.draw(*item.sprite);
        else if (item.text)
            window.draw(*item.text);
    }
}

// ================= Init =================

void MainMenuScene::initAssets()
{
    if (!uiFont.openFromFile("asset/ui/fonts/pixel.ttf"))
    {
        std::cout << "[MainMenuScene] FAILED to load font\n";
    }
    else
    {
        std::cout << "[MainMenuScene] Font loaded OK\n";
    }
}


void MainMenuScene::initMenuItems()
{
    items.clear();

    constexpr unsigned int CHAR_SIZE = 16;
    const sf::Vector2f SCALE = {4.f, 4.f};

    // -------- PLAY --------
    {
        MenuItem play;
        play.text.emplace(uiFont);                 // ✅ CHỈ truyền font
        play.text->setString("PLAY");              // ✅ set sau
        play.text->setCharacterSize(CHAR_SIZE);
        play.text->setScale(SCALE);
        play.text->setFillColor(sf::Color::White);
        play.action = [](){};
        items.push_back(std::move(play));
    }

    // -------- LEVEL SELECT --------
    {
        MenuItem level;
        level.text.emplace(uiFont);
        level.text->setString("LEVEL SELECT");
        level.text->setCharacterSize(CHAR_SIZE);
        level.text->setScale(SCALE);
        level.text->setFillColor(sf::Color::White);
        level.action = [](){};
        items.push_back(std::move(level));
    }

    // -------- MUSIC --------
    {
        MenuItem music;
        music.text.emplace(uiFont);
        music.text->setString("MUSIC: ON");
        music.text->setCharacterSize(CHAR_SIZE);
        music.text->setScale(SCALE);
        music.text->setFillColor(sf::Color::White);
        music.action = [this]()
        {
            musicOn = !musicOn;
            auto& item = items[selectedIndex];
            if (item.text)
                item.text->setString(musicOn ? "MUSIC: ON" : "MUSIC: OFF");
        };
        items.push_back(std::move(music));
    }

    // -------- EXIT --------
    {
        MenuItem exit;
        exit.text.emplace(uiFont);
        exit.text->setString("EXIT");
        exit.text->setCharacterSize(CHAR_SIZE);
        exit.text->setScale(SCALE);
        exit.text->setFillColor(sf::Color::White);
        exit.action = [this]()
        {
            window.close();
        };
        items.push_back(std::move(exit));
    }
}

void MainMenuScene::initLayout()
{
    const sf::Vector2f winSize(
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)
    );

    const float centerX = winSize.x * 0.5f;
    const float startY  = winSize.y * 0.3f;
    const float spacing = 90.f;

    for (int i = 0; i < (int)items.size(); ++i)
    {
        if (!items[i].text) continue;

        auto& text = *items[i].text;

        // ⚠️ BẮT BUỘC: đảm bảo bounds đã update
        sf::FloatRect bounds = text.getLocalBounds();

        // Căn giữa text
        text.setOrigin({
            bounds.position.x + bounds.size.x * 0.5f,
            bounds.position.y + bounds.size.y * 0.5f
        });

        // Đặt vị trí
        text.setPosition({
            centerX,
            startY + i * spacing
        });
    }
}

void MainMenuScene::handleKeyboard(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::Up)
    {
        selectedIndex--;
        if (selectedIndex < 0)
            selectedIndex = (int)items.size() - 1;
    }
    else if (key == sf::Keyboard::Key::Down)
    {
        selectedIndex++;
        if (selectedIndex >= (int)items.size())
            selectedIndex = 0;
    }
    else if (key == sf::Keyboard::Key::Enter)
    {
        items[selectedIndex].action();
    }
    else if (key == sf::Keyboard::Key::Escape)
    {
        window.close();
    }

    updateVisual();
}

void MainMenuScene::handleMouseMove(sf::Vector2f)
{
}

void MainMenuScene::handleMouseClick(sf::Vector2f)
{
}

void MainMenuScene::updateVisual()
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        if (!items[i].text) continue;

        if (i == selectedIndex)
            items[i].text->setFillColor(sf::Color::Yellow);
        else
            items[i].text->setFillColor(sf::Color::White);
    }
}
