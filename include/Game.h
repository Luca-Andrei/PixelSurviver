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

    void run();

    void processEvents();

    void update(float deltaTime);

    void render();

    void restartGame();

private:
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

    sf::Event event;

    sf::RectangleShape xpBar;
    sf::RectangleShape xpFill;

    void handleXP();

    void spawnMonsters();

    void attackMonsters();

    static std::vector<std::string> loadFireballTextures();

    void drawHealthBars();
};
#endif
