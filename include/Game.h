#include "Ability.h"
#include "Monster.h"
#include "Hero.h"
// Game.h
class Game {
public:
    Game();

    void run();
    void processEvents();
    void update(float deltaTime);
    void render();
    void restartGame();

private:
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


    void spawnMonsters();
    void attackMonsters();
    std::vector<std::string> loadFireballTextures();
};
