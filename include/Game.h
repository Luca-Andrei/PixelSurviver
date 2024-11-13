#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Hero.h"
#include "Monster.h"

class Game {
public:
    Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void restartGame();
    void spawnMonsters();
    void attackMonsters();

    sf::RenderWindow window;
    Hero hero;
    sf::Texture heroTexture;
    sf::Texture monsterTexture;
    std::vector<Monster> monsters;
    bool gameOver;
    sf::Clock spawnClock;

    sf::Font font;
    sf::RectangleShape restartButton;
    sf::Text restartButtonText;
};

#endif
