#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Hero.h"
#include "Monster.h"
#include "Ability.h"
#include "GameContainer.h"
#include "GameSubject.h"
#include "GameLogger.h"
#include "AbilityFactory.h"

class Game : public GameSubject {
public:
    Game();
    ~Game() override;

    Game(const Game &other);
    Game(Game &&other) noexcept;

    Game &operator=(const Game &other);
    Game &operator=(Game &&other) noexcept;

    void run();

    friend std::ostream &operator<<(std::ostream &os, const Game &game);

private:
    bool gameOver;
    bool isPaused;
    sf::RenderWindow window;
    Hero hero;
    std::vector<Monster> monsters;
    std::vector<Ability> fireballs;
    
    GameContainer<Monster> monsterContainer;
    GameContainer<Ability> abilityContainer;
    
    std::vector<std::unique_ptr<GameLogger>> gameLoggers;
    
    sf::View cameraView;
    
    sf::Font font;
    sf::Text timerText;
    sf::Text levelText;
    sf::RectangleShape restartButton;
    sf::RectangleShape abilityUI;
    sf::RectangleShape xpBar;
    sf::RectangleShape xpFill;
    sf::RectangleShape grassBackground;
    
    sf::Texture restartTexture;
    sf::Texture heroTexture;
    sf::Texture monsterTexture;
    sf::Texture grassTexture;
    sf::Texture fireballIconTexture;
    sf::Sprite fireballIconSprite;
    
    std::map<std::string, sf::Texture> abilityIcons;
    
    sf::Clock gameClock;
    sf::Clock spawnClock;
    sf::Clock fireballCooldown;
    float spawnRadius;
    bool abilityCK;
    
    bool showingAbilitySelection;
    std::vector<std::string> selectedAbilities;
    std::vector<std::string> availableAbilities;
    std::map<std::string, sf::Keyboard::Key> abilityKeybinds;
    std::map<std::string, sf::Clock> abilityCooldowns;
    
    void processEvents();
    void update(float deltaTime);
    void render();
    void spawnMonsters();
    void attackMonsters();
    void handleHeroMovement();

    void restartGame();
    void drawHealthBars();
    void pauseGame();
    void unpauseGame();
    
    void showAbilitySelection();
    void handleAbilitySelection(const sf::Event& event);
    void addAbilityToPool(const std::string& abilityType);
    void selectAbility(const std::string& abilityType);
    void renderAbilityUI();
    static std::string getKeyName(sf::Keyboard::Key key);
    
    static sf::Texture loadAbilityIcons(const std::string &directory);
    void loadAllAbilityIcons();
    
    void addObserver(GameObserver* observer) override;
    void removeObserver(GameObserver* observer) override;
    void notifyObservers(const std::string& eventType, const std::string& eventData) override;
};

#endif
