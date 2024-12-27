#include "Game.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <random>
#include "Error.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "PixelSurvivor", sf::Style::Titlebar | sf::Style::Close),
      gameOver(false) {
    try {
        std::cout << "Game has started!" << std::endl;

        if (!heroTexture.loadFromFile("assets/hero_texture.png")) {
            throw TextureLoadError("Error loading hero texture!");
        }

        hero = Hero(heroTexture, 100);

        hero.setPosition(static_cast<float>(window.getSize().x) / 2.f - hero.getSprite().getGlobalBounds().width / 2,
                         static_cast<float>(window.getSize().y) / 2.f - hero.getSprite().getGlobalBounds().height / 2);

        std::cout << "Hero texture loaded successfully!" << std::endl;

        if (!grassTexture.loadFromFile("assets/grass_1.png")) {
            throw TextureLoadError("Error loading grass texture!");
        }
        grassTexture.setRepeated(true);

        grassBackground.setSize(sf::Vector2f(static_cast<float>(window.getSize().x),
                                             static_cast<float>(window.getSize().y)));
        grassBackground.setTexture(&grassTexture);
        grassBackground.setTextureRect(sf::IntRect(0, 0, static_cast<int>(window.getSize().x),
                                                   static_cast<int>(window.getSize().y)));


        std::cout << "Grass textures loaded successfully!" << std::endl;


        if (!monsterTexture.loadFromFile("assets/monster_texture.png")) {
            throw TextureLoadError("Error loading monster texture!");
        }
        std::cout << "Monster texture loaded successfully!" << std::endl;

        restartButton.setSize(sf::Vector2f(200, 50));
        restartButton.setFillColor(sf::Color::Red);
        restartButton.setPosition(300, 250);

        if (!restartTexture.loadFromFile("assets/restartbtn.png")) {
            throw TextureLoadError("Error loading restart button texture!");
        }
        std::cout << "Restart button texture loaded successfully!" << std::endl;

        if (!fireballIconTexture.loadFromFile("assets/Fireball/FireballIcon.png")) {
            throw TextureLoadError("Error loading fireball icon texture!");
        } else {
            fireballIconSprite.setTexture(fireballIconTexture);
        }
        std::cout << "Fireball texture loaded successfully!" << std::endl;

        abilityContainer.setSize(sf::Vector2f(fireballIconSprite.getTexture()->getSize()));
        abilityContainer.setPosition(sf::Vector2f(10.0f, 10.0f));
        abilityContainer.setFillColor(sf::Color(0, 0, 0, 80));
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

        if (!font.loadFromFile("assets/timerfont.ttf")) {
            throw TextureLoadError("Error loading font!");
        }

        timerText.setFont(font);
        timerText.setCharacterSize(36);
        timerText.setFillColor(sf::Color::White);
        float textWidth = timerText.getLocalBounds().width;
        timerText.setPosition((static_cast<float>(window.getSize().x) - textWidth) / 2.f, 5.f);
        std::cout << "Timer started successfully!" << std::endl;

        cameraView.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
        cameraView.setCenter(hero.getPosition().x + hero.getSprite().getGlobalBounds().width / 2,
                             hero.getPosition().y + hero.getSprite().getGlobalBounds().height / 2);
        window.setView(cameraView);

    } catch (const TextureLoadError &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
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
        }
    } catch (const std::exception &e) {
        std::cerr << "Error processing events: " << e.what() << std::endl;
    }
}

void Game::handleXP() {
    try {
        int xpAmount = 10;
        hero.addXP(xpAmount);

        float xpPercentage = static_cast<float>(hero.getXP()) / static_cast<float>(hero.getMaxXP());
        xpFill.setSize(sf::Vector2f(xpBar.getSize().x * xpPercentage, xpFill.getSize().y));
    } catch (const std::exception &e) {
        std::cerr << "Error handling XP: " << e.what() << std::endl;
    }
}

void Game::update(float deltaTime) {
    try {
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

        cameraView.setCenter(hero.getPosition().x + hero.getSprite().getGlobalBounds().width / 2,
                             hero.getPosition().y + hero.getSprite().getGlobalBounds().height / 2);
        window.setView(cameraView);

        if (spawnClock.getElapsedTime().asSeconds() >= 5.0f) {
            spawnMonsters();
            spawnClock.restart();
        }

        attackMonsters();

        sf::Time elapsed = gameClock.getElapsedTime();
        int minutes = static_cast<int>(elapsed.asSeconds()) / 60;
        int seconds = static_cast<int>(elapsed.asSeconds()) % 60;

        std::stringstream timeStream;
        timeStream << (minutes < 10 ? "0" : "") << minutes << ":" << (seconds < 10 ? "0" : "") << seconds;
        timerText.setString(timeStream.str());


        float textWidth = timerText.getLocalBounds().width;
        timerText.setPosition((static_cast<float>(window.getSize().x) - textWidth) / 2.f, 5.f);

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
    } catch (const std::exception &e) {
        std::cerr << "Error during game update: " << e.what() << std::endl;
    }
}

void Game::spawnMonsters() {
    try {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(2, 7);

        int numMonsters = dis(gen);

        std::cout << "A pack of " << numMonsters << " monsters has just spawned!" << std::endl;

        spawnRadius = 300.f;

        for (int i = 0; i < numMonsters; ++i) {
            std::uniform_real_distribution<float> disX(hero.getPosition().x - spawnRadius,
                                                       hero.getPosition().x + spawnRadius);
            std::uniform_real_distribution<float> disY(hero.getPosition().y - spawnRadius, hero.getPosition().y + spawnRadius);

            float x = disX(gen);
            float y = disY(gen);

            monsters.emplace_back(monsterTexture, 50, 5);
            monsters.back().getSprite().setPosition(x, y);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error spawning monsters: " << e.what() << std::endl;
    }
}


void Game::attackMonsters() {
    try {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && fireballCooldown.getElapsedTime().asSeconds() >= 5.f) {
            std::cout << "Fireball used against enemies!" << std::endl;
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            fireballs.emplace_back(loadFireballTextures(), 0.04f);
            fireballs.back().trigger(mousePos);
            fireballCooldown.restart();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error attacking monsters: " << e.what() << std::endl;
    }
}

void Game::render() {
    try {
        sf::Vector2u windowSize = window.getSize();

        // Draw the world tiles
        int tileWidth = static_cast<int>(grassTexture.getSize().x) / 10;
        int tileHeight = static_cast<int>(grassTexture.getSize().y) / 10;

        int numTilesX = static_cast<int>(std::ceil(static_cast<int>(windowSize.x) / tileWidth) + 1);
        int numTilesY = static_cast<int>(std::ceil(static_cast<int>(windowSize.y) / tileHeight) + 1);

        int offsetX = static_cast<int>(cameraView.getCenter().x) - static_cast<int>(windowSize.x) / 2;
        int offsetY = static_cast<int>(cameraView.getCenter().y) - static_cast<int>(windowSize.y) / 2;

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

        for (const auto &monster : monsters) {
            monster.draw(window);
        }

        for (const auto &fireball : fireballs) {
            fireball.draw(window);
        }

        hero.draw(window);

        window.setView(window.getDefaultView());

        fireballIconSprite.setPosition(abilityContainer.getPosition());
        window.draw(fireballIconSprite);

        if (fireballCooldown.getElapsedTime().asSeconds() < 5.f) {
            float cooldownTimeLeft = 5.f - fireballCooldown.getElapsedTime().asSeconds();
            float progress = cooldownTimeLeft / 5.f;

            sf::RectangleShape cooldownProgress(sf::Vector2f(abilityContainer.getSize().x,
                                                             abilityContainer.getSize().y * progress));
            cooldownProgress.setFillColor(sf::Color(255, 0, 0, 200));

            cooldownProgress.setPosition(abilityContainer.getPosition().x,
                                         abilityContainer.getPosition().y +
                                         (abilityContainer.getSize().y - cooldownProgress.getSize().y));

            window.draw(cooldownProgress);
        }

        window.draw(timerText);
        window.draw(abilityContainer);
        window.draw(xpBar);
        window.draw(xpFill);

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
    } catch (const std::exception &e) {
        std::cerr << "Error during render: " << e.what() << std::endl;
    }
}







void Game::restartGame() {
    try {
        hero = Hero(heroTexture, 100);
        hero.setPosition(static_cast<float>(window.getSize().x) / 2.f - hero.getSprite().getGlobalBounds().width / 2,
                         static_cast<float>(window.getSize().y) / 2.f - hero.getSprite().getGlobalBounds().height / 2);
        monsters.clear();
        fireballs.clear();
        gameOver = false;
        spawnClock.restart();
        fireballCooldown.restart();
    } catch (const std::exception &e) {
        std::cerr << "Error restarting game: " << e.what() << std::endl;
    }
}

void Game::drawHealthBars() {
    try {
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

            float monsterHealthPercentage = static_cast<float>(monster.getHealth()) / static_cast<float>(monster.getMaxHealth());
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
    } catch (const std::exception &e) {
        std::cerr << "Error drawing health bars: " << e.what() << std::endl;
    }
}

std::vector<std::string> Game::loadFireballTextures() {
    try {
        std::vector<std::string> fireballTextures;
        for (int i = 1; i <= 12; ++i) {
            std::string fileName = "assets/Fireball/Fireball" + std::string(i < 10 ? "000" : "00") + std::to_string(i) + ".png";
            fireballTextures.push_back(fileName);
        }
        return fireballTextures;
    } catch (const std::exception &e) {
        std::cerr << "Error loading fireball textures: " << e.what() << std::endl;
        return {};
    }
}

Game::~Game() {
    try {
        std::cout << "Game class is being destroyed!" << std::endl;

        heroTexture = sf::Texture();
        std::cout << "Hero texture dropped." << std::endl;

        monsterTexture = sf::Texture();
        monsters.clear();
        std::cout << "Monster texture dropped and monsters cleared." << std::endl;

        restartTexture = sf::Texture();
        restartButton = sf::RectangleShape();
        std::cout << "Restart button texture dropped and reset." << std::endl;

        fireballIconTexture = sf::Texture();
        fireballs.clear();
        std::cout << "Fireball icon texture dropped and fireballs cleared." << std::endl;

        xpBar = sf::RectangleShape();
        xpFill = sf::RectangleShape();
        std::cout << "XP textures dropped." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error cleaning up resources: " << e.what() << std::endl;
    }
}
