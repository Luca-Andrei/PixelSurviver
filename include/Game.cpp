#include "Game.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <random>

Game::Game()
    : window(sf::VideoMode(800, 600), "PixelSurvivor"),
      gameOver(false) {
    std::cout << "Game has started!" << std::endl;

    if (!heroTexture.loadFromFile("assets/hero_texture.png")) {
        std::cerr << "Error loading hero texture!" << std::endl;
    }

    hero = Hero(heroTexture, 100, 10);

    std::cout << "Hero texture loaded successfully!" << std::endl;

    if (!monsterTexture.loadFromFile("assets/monster_texture.png")) {
        std::cerr << "Error loading monster texture!" << std::endl;
    }

    std::cout << "Monster texture loaded successfully!" << std::endl;

    restartButton.setSize(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::Red);
    restartButton.setPosition(300, 250);

    if (!restartTexture.loadFromFile("assets/restartbtn.png")) {
        std::cerr << "Error loading restart button texture!" << std::endl;
    }

    std::cout << "Restart button texture loaded successfully!" << std::endl;

    if (!fireballIconTexture.loadFromFile("assets/Fireball/FireballIcon.png")) {
        std::cerr << "Error loading fireball icon texture!" << std::endl;
    } else {
        fireballIconSprite.setTexture(fireballIconTexture);
    }

    std::cout << "Fireball texture loaded successfully!" << std::endl;

    abilityContainer.setSize(sf::Vector2f(fireballIconSprite.getTexture()->getSize()));
    abilityContainer.setPosition(sf::Vector2f(10.0f, 10.0f));
    abilityContainer.setFillColor(sf::Color(0, 0, 0, 150));
    abilityContainer.setOutlineThickness(2.f);
    abilityContainer.setOutlineColor(sf::Color::White);

    std::cout << "Ability container created successfully!" << std::endl;

    xpBar.setSize(sf::Vector2f(790.f, 20.f));
    xpBar.setPosition(5.f, static_cast<float>(window.getSize().y) - 5.f);
    xpBar.setFillColor(sf::Color(120, 69, 19));

    std::cout << "XP bar texture created successfully!" << std::endl;

    xpFill.setSize(sf::Vector2f(0.f, 20.f));
    xpFill.setPosition(5.f, static_cast<float>(window.getSize().y) - 5.f);
    xpFill.setFillColor(sf::Color(0, 255, 0));

    std::cout << "XP fill texture created successfully!" << std::endl;

    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(static_cast<float>(window.getSize().x) - 200.f, 10.f);

    std::cout << "Timer started successfully!" << std::endl;
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        update(clock.restart().asSeconds());
        render();
    }
}

void Game::processEvents() {
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
    }
}

void Game::handleXP() {
    int xpAmount = 10;
    hero.addXP(xpAmount);

    float xpPercentage = static_cast<float>(hero.getXP()) / static_cast<float>(hero.getMaxXP());
    xpFill.setSize(sf::Vector2f(xpBar.getSize().x * xpPercentage, xpFill.getSize().y));
}

void Game::update(float deltaTime) {
    if (gameOver) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        hero.move(0, -200 * deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        hero.move(0, 200 * deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        hero.move(-200 * deltaTime, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        hero.move(200 * deltaTime, 0);
    }

    if (spawnClock.getElapsedTime().asSeconds() >= 5.0f) {
        spawnMonsters();
        spawnClock.restart();
    }

    attackMonsters();

    sf::Time elapsed = gameClock.getElapsedTime();
    int minutes = static_cast<int>(elapsed.asSeconds()) / 60;
    int seconds = static_cast<int>(elapsed.asSeconds()) % 60;

    std::stringstream timeStream;
    timeStream << "Time: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds;
    timerText.setString(timeStream.str());

    for (auto &fireball: fireballs) {
        fireball.update();
        fireball.checkCollisionWithMonsters(monsters);
    }

    for (auto &monster: monsters) {
        if (monster.getIsDead()) continue;
        monster.moveTowards(hero.getPosition(), deltaTime);
        if (monster.getBounds().intersects(hero.getBounds())) {
            monster.attack(hero);
        }
    }

    std::erase_if(monsters, [&](const Monster &m) {
        if (m.getIsDead()) {
            handleXP();
            return true;
        }
        return false;
    });

    std::erase_if(fireballs, [](const Ability &fireball) { return !fireball.isActive(); });

    if (!hero.isAlive()) {
        gameOver = true;
    }
}

void Game::spawnMonsters() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, 7);

    int numMonsters = dis(gen);

    std::cout << "A pack of " << numMonsters << " monsters has just spawned!" << std::endl;

    for (int i = 0; i < numMonsters; ++i) {
        std::uniform_real_distribution<float> disX(0.0f, static_cast<float>(window.getSize().x - 1));
        std::uniform_real_distribution<float> disY(0.0f, static_cast<float>(window.getSize().y - 1));

        float x = disX(gen);
        float y = disY(gen);
        monsters.emplace_back(monsterTexture, 50, 5);
        monsters.back().getSprite().setPosition(x, y);
    }
}


void Game::attackMonsters() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && fireballCooldown.getElapsedTime().asSeconds() >= 5.f) {
        std::cout << "Fireball used against enemies!" << std::endl;
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        fireballs.emplace_back(loadFireballTextures(), 0.04f);
        fireballs.back().trigger(mousePos);
        fireballCooldown.restart();
    }
}

void Game::render() {
    window.clear(sf::Color(128, 0, 128));

    window.draw(timerText);

    window.draw(abilityContainer);
    hero.draw(window);
    drawHealthBars();

    sf::RectangleShape xpBorder(sf::Vector2f(xpBar.getSize().x + 4.f, xpBar.getSize().y + 4.f));
    xpBorder.setPosition(xpBar.getPosition().x - 2.f, xpBar.getPosition().y - 2.f);
    xpBorder.setFillColor(sf::Color::White);

    window.draw(xpBorder);
    window.draw(xpBar);
    window.draw(xpFill);


    for (const auto &monster: monsters) {
        monster.draw(window);
    }

    for (const auto &fireball: fireballs) {
        fireball.draw(window);
    }

    fireballIconSprite.setPosition(abilityContainer.getPosition());
    window.draw(fireballIconSprite);

    if (fireballCooldown.getElapsedTime().asSeconds() < 5.f) {
        float cooldownTimeLeft = 5.f - fireballCooldown.getElapsedTime().asSeconds();

        float progress = cooldownTimeLeft / 5.f;
        sf::RectangleShape cooldownProgress(sf::Vector2f(abilityContainer.getSize().x,
                                                         abilityContainer.getSize().y * progress));
        cooldownProgress.setFillColor(sf::Color(255, 0, 0, 200));
        cooldownProgress.setPosition(abilityContainer.getPosition().x,
                                     abilityContainer.getPosition().y + (
                                         abilityContainer.getSize().y - cooldownProgress.getSize().y));

        window.draw(cooldownProgress);
    }

    if (gameOver) {
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("You Are Dead!");
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(250, 150);
        window.draw(gameOverText);
        window.draw(restartButton);
    }

    window.display();
}

void Game::restartGame() {
    hero = Hero(heroTexture, 100, 10);
    monsters.clear();
    fireballs.clear();
    gameOver = false;
    spawnClock.restart();
    fireballCooldown.restart();
}

void Game::drawHealthBars() {
    float heroHealthPercentage = static_cast<float>(hero.getHealth()) / static_cast<float>(hero.getMaxHealth());
    float heroLostHealthPercentage = 1.0f - heroHealthPercentage;

    sf::RectangleShape greenHealthBar(sf::Vector2f(50.f * heroHealthPercentage, 5.f));
    greenHealthBar.setFillColor(sf::Color(0, 255, 0));

    sf::RectangleShape redHealthBar(sf::Vector2f(50.f * heroLostHealthPercentage, 5.f));
    redHealthBar.setFillColor(sf::Color(255, 0, 0));

    float heroXPos = hero.getPosition().x + hero.getSprite().getGlobalBounds().width / 2 - 25.f;
    float heroYPos = hero.getPosition().y - 10.f;

    greenHealthBar.setPosition(heroXPos, heroYPos);
    redHealthBar.setPosition(heroXPos + greenHealthBar.getSize().x, heroYPos);

    window.draw(greenHealthBar);
    window.draw(redHealthBar);

    for (auto &monster: monsters) {
        if (monster.getIsDead()) continue;

        float monsterHealthPercentage = static_cast<float>(monster.getHealth()) / static_cast<float>(monster.
                                            getMaxHealth());
        float monsterLostHealthPercentage = 1.0f - monsterHealthPercentage;

        sf::RectangleShape monsterGreenHealthBar(sf::Vector2f(50.f * monsterHealthPercentage, 5.f));
        monsterGreenHealthBar.setFillColor(sf::Color(0, 255, 0));

        sf::RectangleShape monsterRedHealthBar(sf::Vector2f(50.f * monsterLostHealthPercentage, 5.f));
        monsterRedHealthBar.setFillColor(sf::Color(255, 0, 0));

        float monsterXPos = monster.getPosition().x + monster.getSprite().getGlobalBounds().width / 2 - 25.f;
        float monsterYPos = monster.getPosition().y - 10.f;

        monsterGreenHealthBar.setPosition(monsterXPos, monsterYPos);
        monsterRedHealthBar.setPosition(monsterXPos + monsterGreenHealthBar.getSize().x, monsterYPos);

        window.draw(monsterGreenHealthBar);
        window.draw(monsterRedHealthBar);
    }
}

std::vector<std::string> Game::loadFireballTextures() {
    std::vector<std::string> fireballTextures;
    for (int i = 1; i <= 12; ++i) {
        std::string fileName = "assets/Fireball/Fireball" + std::string(i < 10 ? "000" : "00") + std::to_string(i) +
                               ".png";
        fireballTextures.push_back(fileName);
    }
    return fireballTextures;
}
