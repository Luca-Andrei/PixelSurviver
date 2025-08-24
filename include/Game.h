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
#include "GameUtils.h"
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
    
    // Template Class: GameContainer for monsters and abilities
    GameContainer<Monster> monsterContainer;
    GameContainer<Ability> abilityContainer;
    
    // Observer Pattern: GameLogger observers
    std::vector<std::unique_ptr<GameLogger>> gameLoggers;
    
    // Camera and view
    sf::View cameraView;
    
    // UI elements
    sf::Font font;
    sf::Text timerText;
    sf::Text levelText;
    sf::RectangleShape restartButton;
    sf::RectangleShape abilityUI;
    sf::RectangleShape xpBar;
    sf::RectangleShape xpFill;
    sf::RectangleShape grassBackground;
    
    // Textures
    sf::Texture restartTexture;
    sf::Texture heroTexture;
    sf::Texture monsterTexture;
    sf::Texture grassTexture;
    sf::Texture fireballIconTexture;
    sf::Sprite fireballIconSprite;
    
    // Ability icons
    std::map<std::string, sf::Texture> abilityIcons;
    
    // Game state
    sf::Clock gameClock;
    sf::Clock spawnClock;
    sf::Clock fireballCooldown;
    float spawnRadius;
    bool abilityCK;
    
    // Ability selection system
    bool showingAbilitySelection;
    std::vector<std::string> selectedAbilities;
    std::vector<std::string> availableAbilities;
    std::map<std::string, sf::Keyboard::Key> abilityKeybinds;
    std::map<std::string, sf::Clock> abilityCooldowns;
    
    // Game methods
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
    
    // Ability system methods
    void showAbilitySelection();
    void handleAbilitySelection(const sf::Event& event);
    void addAbilityToPool(const std::string& abilityType);
    void selectAbility(const std::string& abilityType);
    void renderAbilityUI();
    static std::string getKeyName(sf::Keyboard::Key key);
    
    // Utility methods

    static sf::Texture loadAbilityIcons(const std::string &directory);
    void loadAllAbilityIcons();
    
    // Observer pattern methods
    void addObserver(GameObserver* observer) override;
    void removeObserver(GameObserver* observer) override;
    void notifyObservers(const std::string& eventType, const std::string& eventData) override;
};

#endif // GAME_H
