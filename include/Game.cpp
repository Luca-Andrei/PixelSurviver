#include "Game.h"
#include <cmath>
#include <iostream>

Game::Game()
    : window(sf::VideoMode(800, 600), "PixelSurvivor"),
      gameOver(false) {
    std::cout << "Game has started!" << std::endl;

    if (!heroTexture.loadFromFile("assets/hero_texture.png")) {
        std::cerr << "Error loading hero texture!" << std::endl;
    }
    hero = Hero(heroTexture, 100, 10);

    if (!monsterTexture.loadFromFile("assets/monster_texture.png")) {
        std::cerr << "Error loading monster texture!" << std::endl;
    }

    restartButton.setSize(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::Red);
    restartButton.setPosition(300, 250);

    if (!restartTexture.loadFromFile("assets/restartbtn.png")) {
        std::cerr << "Error loading restart texture!" << std::endl;
    }

    if (!fireballIconTexture.loadFromFile("assets/Fireball/FireballIcon.png")) {
        std::cerr << "Error loading fireball icon texture!" << std::endl;
    } else {
        fireballIconSprite.setTexture(fireballIconTexture);
    }

    abilityContainer.setSize(sf::Vector2f(10.f, 10.f));
    abilityContainer.setPosition(sf::Vector2f(10.0f, 10.0f));
    abilityContainer.setFillColor(sf::Color(0, 0, 0, 150));
    abilityContainer.setOutlineThickness(2.f);
    abilityContainer.setOutlineColor(sf::Color::White);

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

    for (auto &fireball : fireballs) {
        fireball.update();
        fireball.checkCollisionWithMonsters(monsters);
    }

    for (auto &monster : monsters) {
        if (monster.getIsDead()) continue;
        monster.moveTowards(hero.getPosition(), deltaTime);
        if (monster.getBounds().intersects(hero.getBounds())) {
            monster.attack(hero);
        }
    }

    monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                  [](Monster &m) { return m.getIsDead(); }), monsters.end());

    fireballs.erase(std::remove_if(fireballs.begin(), fireballs.end(),
                                    [](const Ability &fireball) { return !fireball.isActive(); }), fireballs.end());

    if (!hero.isAlive() && !gameOver) {
        gameOver = true;
    }
}

void Game::spawnMonsters() {
    int numMonsters = rand() % 6 + 2;
    for (int i = 0; i < numMonsters; ++i) {
        int x = rand() % window.getSize().x;
        int y = rand() % window.getSize().y;
        monsters.emplace_back(monsterTexture, 50, 5);
        monsters.back().getSprite().setPosition(x, y);
    }
}

void Game::attackMonsters() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && fireballCooldown.getElapsedTime().asSeconds() >= 5.f) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        fireballs.push_back(Ability(loadFireballTextures(), 0.04f));
        fireballs.back().trigger(mousePos);
        fireballCooldown.restart();
    }
}

void Game::render() {
    window.clear(sf::Color(128, 0, 128));

    window.draw(abilityContainer);

    hero.draw(window);

    for (auto &monster : monsters) {
        monster.draw(window);
    }

    for (auto &fireball : fireballs) {
        fireball.draw(window);
    }

    fireballIconSprite.setPosition(abilityContainer.getPosition());
    window.draw(fireballIconSprite);

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


std::vector<std::string> Game::loadFireballTextures() {
    std::vector<std::string> fireballTextures;
    for (int i = 1; i <= 12; ++i) {
        std::string fileName = "assets/Fireball/Fireball" + std::string(i < 10 ? "000" : "00") + std::to_string(i) + ".png";
        fireballTextures.push_back(fileName);
    }
    return fireballTextures;
}

