#include "scene/PlayScene.hpp"
#include "scene/SceneManager.hpp"
#include <iostream>
#include "system/AudioManager.hpp"
#include "system/SaveManager.hpp"
#include "scene/PauseScene.hpp"
#include "ui/HUD.hpp"
#include "scene/GameOverScene.hpp"
#include "scene/LevelCompleteScene.hpp"
#include "scene/EndGameScene.hpp"

PlayScene::PlayScene(SceneManager& mgr, sf::RenderWindow& win, int startLevel)
: Scene(mgr)
, window(win)
, game(window.getSize().x, window.getSize().y)
{
    game.bindWindow(window);
    game.startLevel(startLevel);
}

void PlayScene::handleEvent(const sf::Event& e)
{
    // Pause on Escape key
    if (e.is<sf::Event::KeyPressed>() &&
        e.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
    {
        manager.push(std::make_unique<PauseScene>(manager, window));
        return;
    }
}

void PlayScene::update(float dt)
{
    game.update(dt);

    // ===== GAME OVER (CHẶN NGAY) =====
    if (game.getLife() <= 0)
    {
        manager.change(
            std::make_unique<GameOverScene>(
                manager,
                window,
                game.getCurrentLevel(),
                game.getCoin()
            )
        );
        return;
    }
    // ===== END GAME =====
    if (game.isLevelCompleted() && game.isEndGame())
    {
        manager.change(
            std::make_unique<EndGameScene>(manager, window)
        );
        return;
    }
    // ===== LEVEL COMPLETE =====
    if (game.isLevelCompleted())
    {
        SaveManager::get().unlockLevel(game.getCurrentLevel() + 1);

        manager.change(
            std::make_unique<LevelCompleteScene>(
                manager,
                window,
                game.getCurrentLevel(),
                game.getCoin()
            )
        );
        return;
    }
    hud.setLife(game.getLife());
    hud.setCoin(game.getCoin());
}

void PlayScene::render(sf::RenderWindow& window)
{
    static bool once = false;
    if (!once)
    {
        std::cout << "[PlayScene] render()\n";
        once = true;
    }
    game.render();
    hud.draw(window);
}

void PlayScene::onEnter()
{
    AudioManager::get().playBackground();
}

void PlayScene::onExit()
{
    AudioManager::get().stopBackground();
}