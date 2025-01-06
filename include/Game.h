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

    ~Game();

    void run();

    void processEvents();

    void update(float deltaTime);

    void render();

    void restartGame();

    void pauseGame();

    void showAbilitySelection();

    void unpauseGame();

    bool gameOver;
    bool isPaused;

    friend std::ostream &operator<<(std::ostream &os, const Game &game);

    Game &operator=(const Game &other);

    Game(const Game &other);

private:
    sf::RenderWindow window;
    sf::View cameraView;

    sf::Font font;

    Hero hero;
    std::vector<Monster> monsters;
    std::vector<Ability> fireballs;

    sf::Text timerText;
    sf::RectangleShape restartButton;
    sf::RectangleShape abilityContainer;
    sf::RectangleShape xpBar;
    sf::RectangleShape xpFill;
    sf::RectangleShape grassBackground;

    sf::Texture restartTexture;
    sf::Texture heroTexture;
    sf::Texture monsterTexture;
    sf::Texture grassTexture;

    sf::Texture fireballIconTexture;
    sf::Sprite fireballIconSprite;

    sf::Clock gameClock;
    sf::Clock spawnClock;
    sf::Clock fireballCooldown;

    float spawnRadius;
    bool abilityCK;

    void handleXP();

    void spawnMonsters();

    void attackMonsters();

    static std::vector<std::string> loadAbilityTextures(const std::string &directory, int numTextures);

    static sf::Texture loadAbilityIcons(const std::string &directory);

    void drawHealthBars();
};

#endif
