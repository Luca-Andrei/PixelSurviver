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
    void showAbilitySelection();
    void unpauseGame();

private:
    sf::RenderWindow window;
    sf::View cameraView;

    sf::Font font;

    bool gameOver;
    bool isPaused;

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

    void drawHealthBars();

    void loadAbilitiesFromFile(const std::string& filePath);

    struct AbilityData {
        int damage;
        float cooldown;
        bool invincibility;
        float invincibilityDuration;
        float power;
        float life_steal;

        explicit AbilityData(int dmg = 0, float cd = 0.0f, bool inv = false, float invDuration = 0.0f, float pow = 0.0f, float ls=0.0f)
            : damage(dmg), cooldown(cd), invincibility(inv), invincibilityDuration(invDuration), power(pow), life_steal(ls) {}
    };

    std::map<std::string, AbilityData> abilities;
};

#endif
