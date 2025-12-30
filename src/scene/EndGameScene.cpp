#include "scene/EndGameScene.hpp"
#include "scene/SceneManager.hpp"
#include "system/AudioManager.hpp"
#include <iostream>
#include <cstdint>

EndGameScene::EndGameScene(SceneManager& mgr, sf::RenderWindow& win)
: Scene(mgr)
, window(win)
{
    initAssets();
    initPages();
    updateTexts();
}

void EndGameScene::onEnter()
{
    AudioManager::get().stopBackground();

    if (!endMusic.openFromFile("asset/musics/EndGame.mp3"))
    {
        std::cout << "[EndGame] Failed to load EndGame.mp3\n";
    }
    else
    {
        endMusic.setLooping(true);
        endMusic.play();
    }
}

void EndGameScene::onExit()
{
    endMusic.stop();
}


// ------------------------------------------------

void EndGameScene::handleEvent(const sf::Event&)
{
    // ❌ disable all input
}

// ------------------------------------------------

void EndGameScene::update(float dt)
{
    timer += dt;

    switch (state)
    {
    case State::ShowPage:
        if (timer >= PAGE_DURATION)
        {
            timer = 0.f;
            currentPage++;

            if (currentPage >= (int)pages.size())
            {
                state = State::FadeOut;
            }
            else
            {
                updateTexts();
            }
        }
        break;

    case State::FadeOut:
        fadeAlpha += (255.f / FADE_DURATION) * dt;
        if (fadeAlpha >= 255.f)
        {
            fadeAlpha = 255.f;
            endMusic.stop();        // 🔇 tắt nhạc khi màn hình đã đen
            state = State::Exit;
            timer = 0.f;
        }
        break;

    case State::Exit:
        timer += dt;
        if (timer >= 10.0f)
        {
            window.close();
        }
        break;
    }
}

// ------------------------------------------------

void EndGameScene::render(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());

    window.draw(blackBg);

    if (textEN) window.draw(*textEN);
    if (textVI) window.draw(*textVI);

    if (state != State::ShowPage)
    {
        sf::RectangleShape fade;
        fade.setSize({
            (float)window.getSize().x,
            (float)window.getSize().y
        });
        fade.setFillColor(
            sf::Color(0, 0, 0, static_cast<std::uint8_t>(fadeAlpha))
        );
        window.draw(fade);
    }
}

// ------------------------------------------------

void EndGameScene::initAssets()
{
    if (!uiFont.openFromFile("asset/ui/fonts/pixel.ttf"))
        std::cout << "[EndGame] Font load failed\n";

    blackBg.setSize({
        (float)window.getSize().x,
        (float)window.getSize().y
    });
    blackBg.setFillColor(sf::Color::Black);
}

// ------------------------------------------------

void EndGameScene::initPages()
{
    pages = {
        { "The game is over!", "(Het game roi!)" },
        { "What were you expecting?", "(Ban dang trong doi cai gi vay?)" },
        { "A glorious ending?", "(Mot cai ket that hoanh trang sao?)" },
        { "There is no ending. :)", "(Khong co ket dau nhe.)" },
        { "The dev had to go to school.", "(Dang lam game do thi dev phai di hoc roi.)" },
        { "Bye bye.", "(Bai bai.)" }
    };
}

// ------------------------------------------------

void EndGameScene::updateTexts()
{
    const float cx = window.getSize().x * 0.5f;
    const float cy = window.getSize().y * 0.5f;

    textEN.emplace(uiFont, pages[currentPage].en, 44);
    textEN->setFillColor(sf::Color::White);

    auto b1 = textEN->getLocalBounds();
    textEN->setOrigin({
        b1.position.x + b1.size.x * 0.5f,
        b1.position.y + b1.size.y * 0.5f
    });
    textEN->setPosition({ cx, cy - 20.f });

    textVI.emplace(uiFont, pages[currentPage].vi, 20);
    textVI->setFillColor(sf::Color(255, 215, 0));

    auto b2 = textVI->getLocalBounds();
    textVI->setOrigin({
        b2.position.x + b2.size.x * 0.5f,
        b2.position.y + b2.size.y * 0.5f
    });
    textVI->setPosition({ cx, cy + 30.f });
}