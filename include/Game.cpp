#include "Game.h"
#include <cmath>
#include <iostream>

Game::Game()
    : window(sf::VideoMode(800, 600), "PixelSurvivor"),
      gameOver(false) {
    if (!heroTexture.loadFromFile("../assets/hero_texture.png")) {
        std::cerr << "Error loading hero texture!" << std::endl;
    }

    hero = Hero(heroTexture, 100, 10);

    if (!monsterTexture.loadFromFile("../assets/monster_texture.png")) {
        std::cerr << "Error loading monster texture!" << std::endl;
    }

    restartButton.setSize(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::White);
    restartButton.setPosition(300, 250);

    restartButtonText.setString("Restart");
    restartButtonText.setCharacterSize(24);
    restartButtonText.setFillColor(sf::Color::Red);
    restartButtonText.setPosition(350, 260);
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
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (gameOver && event.type == sf::Event::MouseButtonPressed) {
            if (restartButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                restartGame();
            }
        }
    }
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

    for (auto &monster: monsters) {
        if (monster.getIsDead()) continue; // Skip dead monsters
        monster.moveTowards(hero.getPosition(), deltaTime);
        if (monster.getBounds().intersects(hero.getBounds())) {
            monster.attack(hero);
        }
    }

    if (!hero.isAlive() && !gameOver) {
        gameOver = true;
    }

    monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                  [](Monster &m) { return m.getIsDead(); }), monsters.end());
}

void Game::spawnMonsters() {
    int numMonsters;

    int elapsedTime = round(spawnClock.getElapsedTime().asSeconds());

    switch (elapsedTime / 30) {
        case 0:
            numMonsters = rand() % 2 + 2;
            break;
        case 1:
            numMonsters = rand() % 2 + 6;
            break;
        default:
            numMonsters = rand() % 2 + 6;
            break;
    }

    for (int i = 0; i < numMonsters; ++i) {
        int x = rand() % window.getSize().x;
        int y = rand() % window.getSize().y;
        monsters.emplace_back(monsterTexture, 50, 5);
        monsters.back().getSprite().setPosition(x, y);
    }
}

void Game::attackMonsters() {
    for (auto &monster: monsters) {
        if (monster.getBounds().intersects(hero.getBounds())) {
            monster.takeDamage(5); // Hero attacks the monster
            std::cout << "Monster took 5 damage!" << std::endl;
        }
    }
}

void Game::render() {
    window.clear(sf::Color(128, 0, 128));
    hero.draw(window);
    for (auto &monster: monsters) {
        monster.draw(window);
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
        window.draw(restartButtonText);
    }

    window.display();
}

void Game::restartGame() {
    hero = Hero(heroTexture, 100, 10);
    monsters.clear();
    gameOver = false;
    spawnClock.restart();
}
