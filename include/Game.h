#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Hero.h"
#include "Monster.h"
#include "Ability.h"
#include <vector>

class Game {
public:
    Game();

    virtual ~Game();

    void run();

    void processEvents();

    virtual void update(float deltaTime);

    void render();

    void restartGame();

    void pauseGame();

    bool checkPause() const { return isPaused; }

    void showAbilitySelection();

    void unpauseGame();

private:
    sf::View cameraView;
    sf::Clock gameClock;
    sf::Text timerText;
    sf::RenderWindow window;
    Hero hero;
    std::vector<Monster> monsters;
    std::vector<Ability> fireballs;
    sf::Clock spawnClock;
    sf::Clock fireballCooldown;
    bool gameOver;
    sf::RectangleShape restartButton;
    sf::Texture restartTexture;
    sf::Texture heroTexture;
    sf::Texture monsterTexture;
    sf::Font font;
    sf::Texture fireballIconTexture;
    sf::Sprite fireballIconSprite;
    sf::RectangleShape abilityContainer;
    sf::RectangleShape xpBar;
    sf::RectangleShape xpFill;
    sf::Texture grassTexture;
    sf::RectangleShape grassBackground;
    float spawnRadius{};

    void handleXP();

    void spawnMonsters();

    void attackMonsters();

    static std::vector<std::string> loadFireballTextures();

    void drawHealthBars();

    bool isPaused;

};

#endif
