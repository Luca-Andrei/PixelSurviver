#include "Game.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <random>
#include <fstream>
#include <memory>
#include "Error.h"
#include "GameLogger.h"
#include "GameUtils.h"

Game::Game() : gameOver(false), isPaused(false), window(sf::VideoMode(1200, 800), "Pixel Survivor"),
                hero(), spawnRadius(200.f), abilityCK(false), showingAbilitySelection(false) {
    
    cameraView = window.getDefaultView();
    
    if (!heroTexture.loadFromFile("assets/hero_texture.png")) {
        std::cerr << "Failed to load hero texture!" << std::endl;
    } else {
        hero.getSprite().setTexture(heroTexture);
        hero.getSprite().setScale(0.1f, 0.1f);
    }
    
    if (!monsterTexture.loadFromFile("assets/monster_texture.png")) {
        std::cerr << "Failed to load monster texture!" << std::endl;
    }
    
    if (!grassTexture.loadFromFile("assets/grass.png")) {
        std::cerr << "Failed to load grass texture!" << std::endl;
    }
    
    if (!restartTexture.loadFromFile("assets/restartbtn.png")) {
        std::cerr << "Failed to load restart button texture!" << std::endl;
    }
    
    if (!font.loadFromFile("assets/timerfont.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
    
    hero.setPosition(static_cast<float>(window.getSize().x) / 2.f - static_cast<float>(hero.getBounds().width) / 2,
                     static_cast<float>(window.getSize().y) / 2.f - static_cast<float>(hero.getBounds().height) / 2);
    
    cameraView.setCenter(hero.getPosition().x + static_cast<float>(hero.getBounds().width) / 2,
                         hero.getPosition().y + static_cast<float>(hero.getBounds().height) / 2);
    
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10, 10);
    
    restartButton.setSize(sf::Vector2f(200, 50));
    restartButton.setPosition(500, 400);
    restartButton.setTexture(&restartTexture);
    
    xpBar.setSize(sf::Vector2f(200, 20));
    xpBar.setPosition(10, static_cast<float>(window.getSize().y) - 40.0f);
    xpBar.setFillColor(sf::Color::Transparent);
    xpBar.setOutlineColor(sf::Color::White);
    xpBar.setOutlineThickness(2);
    
    xpFill.setSize(sf::Vector2f(0, 18));
    xpFill.setPosition(11, static_cast<float>(window.getSize().y) - 39.0f);
    xpFill.setFillColor(sf::Color::Green);
    
    float initialXpPercentage = static_cast<float>(hero.getXP()) / static_cast<float>(hero.getMaxXP());
    xpFill.setSize(sf::Vector2f(static_cast<float>(xpBar.getSize().x) * initialXpPercentage, 18.0f));
    
    levelText.setFont(font);
    levelText.setCharacterSize(18);
    levelText.setFillColor(sf::Color::Yellow);
    levelText.setPosition(220, static_cast<float>(window.getSize().y) - 40.0f);
    
    grassBackground.setSize(sf::Vector2f(2000, 2000));
    grassBackground.setPosition(-1000, -1000);
    grassBackground.setTexture(&grassTexture);
    grassBackground.setTextureRect(sf::IntRect(0, 0, 2000, 2000));
    
    availableAbilities = {"Fireball", "Ice Spire", "Plasma Storm"};
    selectedAbilities = {"Fireball"};
    
    abilityKeybinds["Fireball"] = sf::Keyboard::Num1;
    abilityKeybinds["Ice Spire"] = sf::Keyboard::Num2;
    abilityKeybinds["Plasma Storm"] = sf::Keyboard::Num3;
    
    for (const auto& ability : availableAbilities) {
        abilityCooldowns[ability] = sf::Clock();
    }
    
    loadAllAbilityIcons();
    
    fireballIconTexture = loadAbilityIcons("Fireball");
    fireballIconSprite.setTexture(fireballIconTexture);
    fireballIconSprite.setScale(0.5f, 0.5f);
    
    addObserver(new GameLogger());
    
    std::cout << "Game initialized successfully!" << std::endl;
}

void Game::run() {
    try {
        sf::Clock clock;
        while (window.isOpen()) {
            processEvents();
            update(clock.restart().asSeconds());
            render();
        }
    } catch (const std::exception &e) {
        std::cerr << "An error occurred during the game loop: " << e.what() << std::endl;
    }
}

std::ostream &operator<<(std::ostream &os, const Game &game) {
    os << "running: " << (!game.isPaused ? 'Y' : 'N') << std::endl;
    os << "game over: " << (game.gameOver ? 'Y' : 'N') << std::endl;
    os << "window size: (" << static_cast<float>(game.window.getSize().x) << "),(" << static_cast<float>(game.window.
        getSize().y) << ")" << std::endl;
    os << "is hero alive: " << (game.hero.isAlive() ? 'Y' : 'N') << std::endl;
    os << "hero HP: " << game.hero.getHealth() << std::endl;
    os << "hero xp: " << game.hero.getXP() << std::endl;
    os << "hero level: " << game.hero.getLevel() << std::endl;
    os << "hero position: (" << game.hero.getPosition().x << "),(" << game.hero.getPosition().y << ")" << std::endl;
    return os;
}

Game &Game::operator=(const Game &other) {
    if (this != &other) {
        gameOver = other.gameOver;
        isPaused = other.isPaused;
        hero = other.hero;
        monsters = other.monsters;
        fireballs = other.fireballs;
        monsterContainer = other.monsterContainer;
        abilityContainer = other.abilityContainer;
        cameraView = other.cameraView;
        font = other.font;
        timerText = other.timerText;
        levelText = other.levelText;
        restartButton = other.restartButton;
        abilityUI = other.abilityUI;
        xpBar = other.xpBar;
        xpFill = other.xpFill;
        grassBackground = other.grassBackground;
        restartTexture = other.restartTexture;
        heroTexture = other.heroTexture;
        monsterTexture = other.monsterTexture;
        grassTexture = other.grassTexture;
        fireballIconTexture = other.fireballIconTexture;
        fireballIconSprite = other.fireballIconSprite;
        gameClock = other.gameClock;
        spawnClock = other.spawnClock;
        fireballCooldown = other.fireballCooldown;
        spawnRadius = other.spawnRadius;
        abilityCK = other.abilityCK;
        showingAbilitySelection = other.showingAbilitySelection;
        selectedAbilities = other.selectedAbilities;
        availableAbilities = other.availableAbilities;
        abilityKeybinds = other.abilityKeybinds;
        abilityCooldowns = other.abilityCooldowns;
        
        gameLoggers.clear();
        for (const auto& logger : other.gameLoggers) {
            gameLoggers.push_back(std::make_unique<GameLogger>(*logger));
        }
        
        std::cout << "Game copy assignment operator called." << std::endl;
    }
    return *this;
}

Game &Game::operator=(Game &&other) noexcept {
    if (this != &other) {
        gameOver = other.gameOver;
        isPaused = other.isPaused;
        hero = std::move(other.hero);
        monsters = std::move(other.monsters);
        fireballs = std::move(other.fireballs);
        monsterContainer = std::move(other.monsterContainer);
        abilityContainer = std::move(other.abilityContainer);
        cameraView = other.cameraView;
        font = other.font;
        timerText = other.timerText;
        levelText = other.levelText;
        restartButton = other.restartButton;
        abilityUI = other.abilityUI;
        xpBar = other.xpBar;
        xpFill = other.xpFill;
        grassBackground = other.grassBackground;
        restartTexture = other.restartTexture;
        heroTexture = other.heroTexture;
        monsterTexture = other.monsterTexture;
        grassTexture = other.grassTexture;
        fireballIconTexture = other.fireballIconTexture;
        fireballIconSprite = other.fireballIconSprite;
        gameClock = other.gameClock;
        spawnClock = other.spawnClock;
        fireballCooldown = other.fireballCooldown;
        spawnRadius = other.spawnRadius;
        abilityCK = other.abilityCK;
        showingAbilitySelection = other.showingAbilitySelection;
        selectedAbilities = std::move(other.selectedAbilities);
        availableAbilities = std::move(other.availableAbilities);
        abilityKeybinds = std::move(other.abilityKeybinds);
        abilityCooldowns = std::move(other.abilityCooldowns);
        gameLoggers = std::move(other.gameLoggers);
        
        std::cout << "Game move assignment operator called." << std::endl;
    }
    return *this;
}

Game::Game(const Game &other) : GameSubject(other), gameOver(other.gameOver), isPaused(other.isPaused),
                                  window(sf::VideoMode(1200, 800), "Pixel Survivor"),
                                  hero(other.hero), monsters(other.monsters), fireballs(other.fireballs),
                                  monsterContainer(other.monsterContainer), abilityContainer(other.abilityContainer),
                                  gameLoggers(),                                   cameraView(other.cameraView), font(other.font), timerText(other.timerText),
                                  levelText(other.levelText), restartButton(other.restartButton), abilityUI(other.abilityUI),
                                  xpBar(other.xpBar), xpFill(other.xpFill), grassBackground(other.grassBackground),
                                  restartTexture(other.restartTexture), heroTexture(other.heroTexture),
                                  monsterTexture(other.monsterTexture), grassTexture(other.grassTexture),
                                  fireballIconTexture(other.fireballIconTexture), fireballIconSprite(other.fireballIconSprite),
                                  gameClock(other.gameClock), spawnClock(other.spawnClock), fireballCooldown(other.fireballCooldown),
                                  spawnRadius(other.spawnRadius), abilityCK(other.abilityCK),
                                  showingAbilitySelection(other.showingAbilitySelection),
                                  selectedAbilities(other.selectedAbilities), availableAbilities(other.availableAbilities),
                                  abilityKeybinds(other.abilityKeybinds), abilityCooldowns(other.abilityCooldowns) {
    
    for (const auto& logger : other.gameLoggers) {
        gameLoggers.push_back(std::make_unique<GameLogger>(*logger));
    }
    
    std::cout << "Game copy constructor called." << std::endl;
}

Game::Game(Game &&other) noexcept : gameOver(other.gameOver), isPaused(other.isPaused),
                                       hero(std::move(other.hero)),
                                       monsters(std::move(other.monsters)), fireballs(std::move(other.fireballs)),
                                       monsterContainer(std::move(other.monsterContainer)),
                                       abilityContainer(std::move(other.abilityContainer)),
                                       gameLoggers(std::move(other.gameLoggers)),
                                       cameraView(other.cameraView), font(other.font),
                                       timerText(other.timerText), levelText(other.levelText), restartButton(other.restartButton),
                                       abilityUI(other.abilityUI), xpBar(other.xpBar),
                                       xpFill(other.xpFill), grassBackground(other.grassBackground),
                                       restartTexture(other.restartTexture),
                                       heroTexture(other.heroTexture),
                                       monsterTexture(other.monsterTexture),
                                       grassTexture(other.grassTexture),
                                       fireballIconTexture(other.fireballIconTexture),
                                       fireballIconSprite(other.fireballIconSprite),
                                       gameClock(other.gameClock), spawnClock(other.spawnClock),
                                       fireballCooldown(other.fireballCooldown), spawnRadius(other.spawnRadius),
                                       abilityCK(other.abilityCK), showingAbilitySelection(other.showingAbilitySelection),
                                       selectedAbilities(std::move(other.selectedAbilities)),
                                       availableAbilities(std::move(other.availableAbilities)),
                                       abilityKeybinds(std::move(other.abilityKeybinds)),
                                       abilityCooldowns(std::move(other.abilityCooldowns)) {
    
    std::cout << "Game move constructor called." << std::endl;
}

void Game::pauseGame() {
    std::cout << "Game has been paused!" << std::endl;
    isPaused = true;
}

void Game::unpauseGame() {
    std::cout << "Game has been unpaused!" << std::endl;
    isPaused = false;
}

void Game::showAbilitySelection() {
    try {
        std::cout << "Showing ability selection screen" << std::endl;
        float windowWidth = static_cast<float>(window.getSize().x);
        float windowHeight = static_cast<float>(window.getSize().y);

        float rectHeight = windowHeight / 2.f;
        float totalHorizontalSpacing = windowWidth / 6.f;
        float rectWidth = (windowWidth - 2 * totalHorizontalSpacing) / 3.f - totalHorizontalSpacing / 3.f;
        float spacing = totalHorizontalSpacing / 3.f;

        sf::RectangleShape abilityOption1(sf::Vector2f(rectWidth, rectHeight));
        sf::RectangleShape abilityOption2(sf::Vector2f(rectWidth, rectHeight));
        sf::RectangleShape abilityOption3(sf::Vector2f(rectWidth, rectHeight));

        abilityOption2.setPosition((windowWidth - rectWidth) / 2.f, (windowHeight - rectHeight) / 2.f);

        abilityOption1.setPosition(abilityOption2.getPosition().x - rectWidth - spacing,
                                   (windowHeight - rectHeight) / 2.f);
        abilityOption3.setPosition(abilityOption2.getPosition().x + rectWidth + spacing,
                                   (windowHeight - rectHeight) / 2.f);

        abilityOption1.setOutlineThickness(2.f);
        abilityOption2.setOutlineThickness(2.f);
        abilityOption3.setOutlineThickness(2.f);

        abilityOption1.setOutlineColor(sf::Color::White);
        abilityOption2.setOutlineColor(sf::Color::White);
        abilityOption3.setOutlineColor(sf::Color::White);

        abilityOption1.setFillColor(sf::Color(50, 50, 50, 150));
        abilityOption2.setFillColor(sf::Color(50, 50, 50, 150));
        abilityOption3.setFillColor(sf::Color(50, 50, 50, 150));

        window.draw(abilityOption1);
        window.draw(abilityOption2);
        window.draw(abilityOption3);

        sf::Text text1, text2, text3;
        text1.setFont(font);
        text2.setFont(font);
        text3.setFont(font);

        text1.setString(availableAbilities[0]);
        text2.setString(availableAbilities[1]);
        text3.setString(availableAbilities[2]);

        text1.setCharacterSize(24);
        text2.setCharacterSize(24);
        text3.setCharacterSize(24);

        text1.setFillColor(sf::Color::White);
        text2.setFillColor(sf::Color::White);
        text3.setFillColor(sf::Color::White);

        sf::FloatRect text1Bounds = text1.getLocalBounds();
        sf::FloatRect text2Bounds = text2.getLocalBounds();
        sf::FloatRect text3Bounds = text3.getLocalBounds();

        text1.setPosition(abilityOption1.getPosition().x + (rectWidth - text1Bounds.width) / 2.f,
                         abilityOption1.getPosition().y + (rectHeight - text1Bounds.height) / 2.f);
        text2.setPosition(abilityOption2.getPosition().x + (rectWidth - text2Bounds.width) / 2.f,
                         abilityOption2.getPosition().y + (rectHeight - text2Bounds.height) / 2.f);
        text3.setPosition(abilityOption3.getPosition().x + (rectWidth - text3Bounds.width) / 2.f,
                         abilityOption3.getPosition().y + (rectHeight - text3Bounds.height) / 2.f);

        window.draw(text1);
        window.draw(text2);
        window.draw(text3);

        sf::Text desc1, desc2, desc3;
        desc1.setFont(font);
        desc2.setFont(font);
        desc3.setFont(font);

        desc1.setString("Fire damage\nMedium range");
        desc2.setString("Ice damage\nFreezes enemies");
        desc3.setString("Plasma damage\nArea of effect");

        desc1.setCharacterSize(16);
        desc2.setCharacterSize(16);
        desc3.setCharacterSize(16);

        desc1.setFillColor(sf::Color::Yellow);
        desc2.setFillColor(sf::Color::Cyan);
        desc3.setFillColor(sf::Color::Magenta);

        desc1.setPosition(abilityOption1.getPosition().x + 10,
                         abilityOption1.getPosition().y + rectHeight - 60);
        desc2.setPosition(abilityOption2.getPosition().x + 10,
                         abilityOption2.getPosition().y + rectHeight - 60);
        desc3.setPosition(abilityOption3.getPosition().x + 10,
                         abilityOption3.getPosition().y + rectHeight - 60);

        window.draw(desc1);
        window.draw(desc2);
        window.draw(desc3);
    } catch (const std::exception &e) {
        std::cerr << "Error creating ability selection rectangles: " << e.what() << std::endl;
    }
}


void Game::processEvents() {
    try {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (gameOver && event.type == sf::Event::MouseButtonPressed) {
                if (restartButton.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
                                                             static_cast<float>(event.mouseButton.y))) {
                    restartGame();
                }
            }
            
            if (isPaused && showingAbilitySelection && event.type == sf::Event::MouseButtonPressed) {
                std::cout << "Handling ability selection click" << std::endl;
                handleAbilitySelection(event);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error processing events: " << e.what() << std::endl;
    }
}

void Game::handleAbilitySelection(const sf::Event& event) {
    try {
        float windowWidth = static_cast<float>(window.getSize().x);
        float windowHeight = static_cast<float>(window.getSize().y);

        float rectHeight = windowHeight / 2.f;
        float totalHorizontalSpacing = windowWidth / 6.f;
        float rectWidth = (windowWidth - 2 * totalHorizontalSpacing) / 3.f - totalHorizontalSpacing / 3.f;
        float spacing = totalHorizontalSpacing / 3.f;

        sf::Vector2f option2Pos((windowWidth - rectWidth) / 2.f, (windowHeight - rectHeight) / 2.f);
        sf::Vector2f option1Pos(option2Pos.x - rectWidth - spacing, (windowHeight - rectHeight) / 2.f);
        sf::Vector2f option3Pos(option2Pos.x + rectWidth + spacing, (windowHeight - rectHeight) / 2.f);

        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

        sf::FloatRect option1Bounds(option1Pos.x, option1Pos.y, rectWidth, rectHeight);
        sf::FloatRect option2Bounds(option2Pos.x, option2Pos.y, rectWidth, rectHeight);
        sf::FloatRect option3Bounds(option3Pos.x, option3Pos.y, rectWidth, rectHeight);

        if (option1Bounds.contains(mousePos)) {
            selectAbility(availableAbilities[0]);
        } else if (option2Bounds.contains(mousePos)) {
            selectAbility(availableAbilities[1]);
        } else if (option3Bounds.contains(mousePos)) {
            selectAbility(availableAbilities[2]);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error handling ability selection: " << e.what() << std::endl;
    }
}

void Game::addAbilityToPool(const std::string& abilityType) {
    try {
        if (std::find(selectedAbilities.begin(), selectedAbilities.end(), abilityType) == selectedAbilities.end()) {
            selectedAbilities.push_back(abilityType);
            std::cout << "Added " << abilityType << " to ability pool. Total abilities: " << selectedAbilities.size() << std::endl;
        } else {
            std::cout << abilityType << " is already in the ability pool." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error adding ability to pool: " << e.what() << std::endl;
    }
}

void Game::selectAbility(const std::string& abilityType) {
    try {
        addAbilityToPool(abilityType);
        showingAbilitySelection = false;
        
        hero.completeLevelUp();
        
        float xpPercentage = static_cast<float>(hero.getXP()) / static_cast<float>(hero.getMaxXP());
                    xpFill.setSize(sf::Vector2f(static_cast<float>(xpBar.getSize().x) * xpPercentage, 18.0f));
        
        unpauseGame();
        
        notifyObservers("AbilitySelected", "Player added ability to pool: " + abilityType);
        
        std::cout << "Added ability to pool: " << abilityType << std::endl;
        std::cout << "Hero is now level " << hero.getLevel() << " with " << hero.getXP() << "/" << hero.getMaxXP() << " XP" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error selecting ability: " << e.what() << std::endl;
    }
}

void Game::renderAbilityUI() {
    
            float yOffset = 20.0f;
        for (const auto& abilityName : selectedAbilities) {
            if (abilityIcons.find(abilityName) != abilityIcons.end()) {
                sf::Sprite iconSprite(abilityIcons[abilityName]);
                iconSprite.setScale(0.8f, 0.8f);
                iconSprite.setPosition(abilityUI.getPosition().x + 10, abilityUI.getPosition().y + yOffset);
                window.draw(iconSprite);
            }
        
        sf::Text abilityText;
        abilityText.setFont(font);
        abilityText.setCharacterSize(16);
        abilityText.setFillColor(sf::Color::White);
        
        std::string keyName = getKeyName(abilityKeybinds[abilityName]);
        std::string abilityString = abilityName;
        abilityString += " (";
        abilityString += keyName;
        abilityString += ")";
        abilityText.setString(abilityString);
        abilityText.setPosition(abilityUI.getPosition().x + 50, abilityUI.getPosition().y + yOffset + 5);
        window.draw(abilityText);
        
        if (abilityCooldowns.find(abilityName) != abilityCooldowns.end()) {
            float cooldownTime = abilityCooldowns[abilityName].getElapsedTime().asSeconds();
            float cooldownProgress = std::min(cooldownTime / 5.0f, 1.0f);
            
            sf::RectangleShape cooldownBar;
            cooldownBar.setSize(sf::Vector2f(80 * (1.0f - cooldownProgress), 4));
            cooldownBar.setPosition(abilityUI.getPosition().x + 50, abilityUI.getPosition().y + yOffset + 25);
            cooldownBar.setFillColor(sf::Color::Red);
            window.draw(cooldownBar);
        }
        
        yOffset += 60.0f;
    }
}

std::string Game::getKeyName(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Num1: return "1";
        case sf::Keyboard::Num2: return "2";
        case sf::Keyboard::Num3: return "3";
        case sf::Keyboard::Num4: return "4";
        case sf::Keyboard::Num5: return "5";
        case sf::Keyboard::Num6: return "6";
        case sf::Keyboard::Num7: return "7";
        case sf::Keyboard::Num8: return "8";
        case sf::Keyboard::Num9: return "9";
        case sf::Keyboard::Num0: return "0";
        default: return "?";
    }
}



void Game::update(float deltaTime) {
    if (gameOver || isPaused) return;
    
    cameraView.setCenter(hero.getPosition().x + static_cast<float>(hero.getBounds().width) / 2,
                         hero.getPosition().y + static_cast<float>(hero.getBounds().height) / 2);
    
    for (auto &monster : monsters) {
        if (monster.getIsDead()) continue;
        
        monster.moveTowards(hero.getPosition(), deltaTime);
        
        if (monster.getBounds().intersects(hero.getBounds())) {
            monster.attack(hero);
        }
    }

    for (auto &fireball : fireballs) {
        fireball.update();
        fireball.checkCollisionWithMonsters(monsters);
    }
    
    for (auto it = monsters.begin(); it != monsters.end();) {
        if (it->getIsDead()) {
            hero.addXP(10);
            std::cout << "Monster killed! Current XP: " << hero.getXP() << "/" << hero.getMaxXP() << std::endl;
            
            float xpPercentage = static_cast<float>(hero.getXP()) / static_cast<float>(hero.getMaxXP());
            xpFill.setSize(sf::Vector2f(static_cast<float>(xpBar.getSize().x) * xpPercentage, 18.0f));
            
            std::cout << "Checking level up: XP=" << hero.getXP() << ", MaxXP=" << hero.getMaxXP() << std::endl;
            if (hero.getXP() >= hero.getMaxXP()) {
                std::cout << "Hero leveled up! XP: " << hero.getXP() << "/" << hero.getMaxXP() << std::endl;
                std::cout << "Setting showingAbilitySelection to true and pausing game" << std::endl;
                pauseGame();
                showingAbilitySelection = true;
                
                hero.resetXP();
                
                xpPercentage = static_cast<float>(hero.getXP()) / static_cast<float>(hero.getMaxXP());
                xpFill.setSize(sf::Vector2f(static_cast<float>(xpBar.getSize().x) * xpPercentage, 18.0f));
                
                std::cout << "Game should now be paused and ability selection should be visible" << std::endl;
            }
            
            it = monsters.erase(it);
        } else {
            ++it;
        }
    }
    
    if (spawnClock.getElapsedTime().asSeconds() >= 3.f) {
        spawnMonsters();
        spawnClock.restart();
    }
    
    attackMonsters();
    
    handleHeroMovement();
    
    if (!hero.isAlive()) {
        gameOver = true;
        std::cout << "Game Over! Hero has died!" << std::endl;
    }
}

void Game::spawnMonsters() {
    try {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1, 3);
        int numMonsters = dis(gen);

        std::cout << "A pack of " << numMonsters << " monsters has just spawned!" << std::endl;

        spawnRadius = 300.f;

        for (int i = 0; i < numMonsters; ++i) {
            std::uniform_real_distribution<float> disX(hero.getPosition().x - spawnRadius,
                                                       hero.getPosition().x + spawnRadius);
            std::uniform_real_distribution<float> disY(hero.getPosition().y - spawnRadius,
                                                       hero.getPosition().y + spawnRadius);

            float x = disX(gen);
            float y = disY(gen);

            monsters.emplace_back(monsterTexture, 50, 5);
            monsters.back().setPosition(x, y);
            monsters.back().getSprite().setScale(0.1f, 0.1f);
            
            auto monsterPtr = std::make_unique<Monster>(monsters.back());
            monsterContainer.addItem(std::move(monsterPtr));
        }
        
        std::vector<int> monsterHealths;
        for (const auto& monster : monsters) {
            monsterHealths.push_back(monster.getHealth());
        }
        
        if (!monsterHealths.empty()) {
            int avgHealth = calculateAverage(monsterHealths);
            int maxHealth = findMax(monsterHealths, [](int a, int b) { return a < b; });
            
            std::string spawnInfo = "Spawned " + std::to_string(numMonsters) + 
                                   " monsters, avg health: " + std::to_string(avgHealth) + 
                                   ", max health: " + std::to_string(maxHealth);
            notifyObservers("MonsterSpawn", spawnInfo);
        }
        
    } catch (const std::exception &e) {
        std::cerr << "Error spawning monsters: " << e.what() << std::endl;
    }
}


void Game::attackMonsters() {
    try {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!selectedAbilities.empty()) {
                std::string primaryAbility = selectedAbilities[0];
                
                if (abilityCooldowns.find(primaryAbility) != abilityCooldowns.end() && 
                    abilityCooldowns[primaryAbility].getElapsedTime().asSeconds() >= 5.f) {
                    
                    std::cout << primaryAbility << " used against enemies!" << std::endl;
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    
                    auto ability = AbilityFactory::createAbility(primaryAbility);
                    ability->trigger(mousePos);
                    
                    fireballs.emplace_back(*ability);
                    
                    abilityContainer.addItem(std::move(ability));
                    
                    abilityCooldowns[primaryAbility].restart();
                    
                    notifyObservers("AbilityUsed", primaryAbility + " ability triggered at position " + 
                                  std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
                }
            }
        }
        
        for (const auto& abilityName : selectedAbilities) {
            if (abilityKeybinds.find(abilityName) != abilityKeybinds.end()) {
                sf::Keyboard::Key key = abilityKeybinds[abilityName];
                if (sf::Keyboard::isKeyPressed(key)) {
                    if (abilityCooldowns.find(abilityName) != abilityCooldowns.end() && 
                        abilityCooldowns[abilityName].getElapsedTime().asSeconds() >= 5.f) {
                        
                        std::cout << abilityName << " used against enemies!" << std::endl;
                        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        
                        auto ability = AbilityFactory::createAbility(abilityName);
                        ability->trigger(mousePos);
                        
                        fireballs.emplace_back(*ability);
                        
                        abilityContainer.addItem(std::move(ability));
                        
                        abilityCooldowns[abilityName].restart();
                        
                        notifyObservers("AbilityUsed", abilityName + " ability triggered at position " + 
                                      std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
                        break;
                    }
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error attacking monsters: " << e.what() << std::endl;
    }
}

void Game::handleHeroMovement() {
    try {
        float moveSpeed = 25.0f;
        float deltaTime = 1.0f / 60.0f;
        
        sf::Vector2f movement(0.0f, 0.0f);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            movement.y -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            movement.y += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            movement.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            movement.x += 1.0f;
        }
        
        if (movement.x != 0.0f && movement.y != 0.0f) {
            movement = movement / std::sqrt(2.0f);
        }
        
        if (movement.x != 0.0f || movement.y != 0.0f) {
            sf::Vector2f newPosition = hero.getPosition() + movement * moveSpeed * deltaTime;
            hero.setPosition(newPosition.x, newPosition.y);
            
            hero.updateMovementAnimation(movement);
        } else {
            hero.setAnimationState("idle");
        }
        
    } catch (const std::exception &e) {
        std::cerr << "Error handling hero movement: " << e.what() << std::endl;
    }
}

void Game::render() {
    try {
        sf::Vector2u windowSize = window.getSize();

        int tileWidth = static_cast<int>(grassTexture.getSize().x) / 10;
        int tileHeight = static_cast<int>(grassTexture.getSize().y) / 10;

        int numTilesX = static_cast<int>(std::ceil(static_cast<int>(windowSize.x) / tileWidth) + 1);
        int numTilesY = static_cast<int>(std::ceil(static_cast<int>(windowSize.y) / tileHeight) + 1);

        int offsetX = static_cast<int>(cameraView.getCenter().x) - static_cast<int>(windowSize.x) / 2;
        int offsetY = static_cast<int>(cameraView.getCenter().y) - static_cast<int>(windowSize.y) / 2;

        window.setView(cameraView);
        for (int i = 0; i < numTilesX; ++i) {
            for (int j = 0; j < numTilesY; ++j) {
                sf::Sprite grassSprite(grassTexture);
                grassSprite.setScale(.1f, .1f);
                int posX = (i * tileWidth) + offsetX;
                int posY = (j * tileHeight) + offsetY;

                grassSprite.setPosition(static_cast<float>(posX), static_cast<float>(posY));

                window.draw(grassSprite);
            }
        }

        drawHealthBars();

        for (const auto &monster: monsters) {
            monster.draw(window);
        }

        for (const auto &fireball: fireballs) {
            fireball.draw(window);
        }

        hero.draw(window);

        window.setView(window.getDefaultView());

        renderAbilityUI();

        window.draw(timerText);
        window.draw(xpBar);
        window.draw(xpFill);
        
        levelText.setString("Level: " + std::to_string(hero.getLevel()));
        window.draw(levelText);

        if (gameOver) {
            sf::RectangleShape blackBackground(sf::Vector2f(static_cast<float>(window.getSize().x),
                                                            static_cast<float>(window.getSize().y)));
            blackBackground.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(blackBackground);
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("You Are Dead!");
            gameOverText.setCharacterSize(50);
            gameOverText.setFillColor(sf::Color::Red);
            float gameOverWidth = gameOverText.getLocalBounds().width;
            gameOverText.setPosition((static_cast<float>(window.getSize().x) - gameOverWidth) / 2.f, 150.f);
            window.draw(gameOverText);
            window.draw(restartButton);
        }

        if (isPaused && showingAbilitySelection) {
            std::cout << "Rendering ability selection screen - isPaused: " << isPaused << ", showingAbilitySelection: " << showingAbilitySelection << std::endl;
            
            sf::RectangleShape blackBackground(sf::Vector2f(static_cast<float>(window.getSize().x),
                                                            static_cast<float>(window.getSize().y)));
            blackBackground.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(blackBackground);

            sf::Text pauseText;
            pauseText.setFont(font);
            pauseText.setString("Choose your ability!");
            pauseText.setCharacterSize(50);
            pauseText.setFillColor(sf::Color::Yellow);
            float pauseTextWidth = pauseText.getLocalBounds().width;
            pauseText.setPosition((static_cast<float>(window.getSize().x) - pauseTextWidth) / 2.f, 20.f);
            window.draw(pauseText);
            
            showAbilitySelection();
            
            window.display();
            return;
        }

        window.setView(cameraView);

        window.display();
    } catch (const std::exception &e) {
        std::cerr << "Error during render: " << e.what() << std::endl;
    }
}


void Game::restartGame() {
    gameOver = false;
    isPaused = false;
    
    hero.reset();
    hero.setPosition(static_cast<float>(window.getSize().x) / 2.f - static_cast<float>(hero.getBounds().width) / 2,
                     static_cast<float>(window.getSize().y) / 2.f - static_cast<float>(hero.getBounds().height) / 2);
    
    monsters.clear();
    fireballs.clear();
    
    gameClock.restart();
    spawnClock.restart();
    fireballCooldown.restart();
    abilityCK = false;
    
    cameraView.setCenter(hero.getPosition().x + static_cast<float>(hero.getBounds().width) / 2,
                         hero.getPosition().y + static_cast<float>(hero.getBounds().height) / 2);
    
    selectedAbilities = {"Fireball"};
    for (auto& cooldown : abilityCooldowns) {
        cooldown.second.restart();
    }
    
    std::cout << "Game restarted!" << std::endl;
}

void Game::drawHealthBars() {
    try {
        float heroHealthPercentage = static_cast<float>(hero.getHealth()) / static_cast<float>(hero.getMaxHealth());
        float heroLostHealthPercentage = 1.0f - heroHealthPercentage;

        sf::RectangleShape greenHealthBar(sf::Vector2f(50.f * heroHealthPercentage, 5.f));
        greenHealthBar.setFillColor(sf::Color(0, 255, 0));

        sf::RectangleShape redHealthBar(sf::Vector2f(50.f * heroLostHealthPercentage, 5.f));
        redHealthBar.setFillColor(sf::Color(255, 0, 0));

        float heroXPos = hero.getPosition().x + static_cast<float>(hero.getBounds().width) / 2 - 25.f;
        float heroYPos = hero.getPosition().y - 10.f;

        greenHealthBar.setPosition(heroXPos, heroYPos);
        redHealthBar.setPosition(heroXPos + static_cast<float>(greenHealthBar.getSize().x), heroYPos);

        window.draw(greenHealthBar);
        window.draw(redHealthBar);

        for (const auto &monster: monsters) {
            if (monster.getIsDead()) continue;

            float monsterHealthPercentage = static_cast<float>(monster.getHealth()) / static_cast<float>(monster.
                                                getMaxHealth());
            float monsterLostHealthPercentage = 1.0f - monsterHealthPercentage;

            sf::RectangleShape monsterGreenHealthBar(sf::Vector2f(50.f * monsterHealthPercentage, 5.f));
            monsterGreenHealthBar.setFillColor(sf::Color(255, 0, 0));

            sf::RectangleShape monsterRedHealthBar(sf::Vector2f(50.f * monsterLostHealthPercentage, 5.f));
            monsterRedHealthBar.setFillColor(sf::Color(255, 0, 0, 150));

            float monsterXPos = monster.getPosition().x + static_cast<float>(monster.getBounds().width) / 2 - 25.f;
            float monsterYPos = monster.getPosition().y - 10.f;

            monsterGreenHealthBar.setPosition(monsterXPos, monsterYPos);
            monsterRedHealthBar.setPosition(monsterXPos + static_cast<float>(monsterGreenHealthBar.getSize().x), monsterYPos);

            window.draw(monsterGreenHealthBar);
            window.draw(monsterRedHealthBar);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error drawing health bars: " << e.what() << std::endl;
    }
}



sf::Texture Game::loadAbilityIcons(const std::string &directory) {
    sf::Texture iconTexture;
    std::string iconPath = "assets/";
    iconPath += directory;
    iconPath += "/";
    iconPath += directory;
    iconPath += " Icon.PNG";
    
            if (!iconTexture.loadFromFile(iconPath)) {
            std::cerr << "Failed to load icon: " << iconPath << std::endl;
            sf::Image fallbackImage;
            fallbackImage.create(32, 32, sf::Color::Magenta);
            iconTexture.loadFromImage(fallbackImage);
        }
    
    return iconTexture;
}

void Game::loadAllAbilityIcons() {
    try {
        for (const auto& abilityName : availableAbilities) {
            std::string iconPath = "assets/";
            iconPath += abilityName;
            iconPath += "/";
            iconPath += abilityName;
            iconPath += " Icon.PNG";
            sf::Texture iconTexture;
            
            if (iconTexture.loadFromFile(iconPath)) {
                abilityIcons[abilityName] = iconTexture;
                std::cout << "Loaded icon for " << abilityName << std::endl;
            } else {
                std::cerr << "Failed to load icon for " << abilityName << " from " << iconPath << std::endl;
                sf::Image fallbackImage;
                fallbackImage.create(32, 32, sf::Color::Magenta);
                iconTexture.loadFromImage(fallbackImage);
                abilityIcons[abilityName] = iconTexture;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error loading ability icons: " << e.what() << std::endl;
    }
}


Game::~Game() {
    try {
        std::cout << "Game destructor called." << std::endl;

        heroTexture = sf::Texture();
        std::cout << "Hero texture dropped." << std::endl;
        monsterTexture = sf::Texture();
        monsters.clear();
        std::cout << "Monster texture dropped and monsters cleared." << std::endl;
        restartTexture = sf::Texture();
        restartButton = sf::RectangleShape();
        std::cout << "Restart button texture dropped and reset." << std::endl;

        fireballs.clear();
        std::cout << "Fireballs cleared." << std::endl;
        
        gameLoggers.clear();
        std::cout << "Game loggers cleared." << std::endl;
        
        std::cout << "Game destructor completed successfully." << std::endl;
        
    } catch (const std::exception &e) {
        std::cerr << "Error in Game destructor: " << e.what() << std::endl;
    }
}

void Game::addObserver(GameObserver* observer) {
    GameSubject::addObserver(observer);
}

void Game::removeObserver(GameObserver* observer) {
    GameSubject::removeObserver(observer);
}

void Game::notifyObservers(const std::string& eventType, const std::string& eventData) {
    GameSubject::notifyObservers(eventType, eventData);
}
