// WaveManager.cpp
#include "WaveManager.h"

WaveManager::WaveManager(float spawnInterval)
    : spawnInterval(spawnInterval), elapsedTime(0.0f), waveCount(1) {}

WaveManager::~WaveManager() {
    delete boss;  // Eliberăm memoria pentru boss, dacă există
}

void WaveManager::update(float deltaTime, const sf::Vector2f &playerPosition) {
    elapsedTime += deltaTime;

    // Spawnează inamici la intervale regulate
    if (elapsedTime >= spawnInterval) {
        elapsedTime = 0.0f;
        // Creștem numărul de inamici odată cu creșterea numărului de valuri
        for (int i = 0; i < waveCount; ++i) {
            sf::Texture enemyTexture;  // Aici ar trebui să fie texturile corespunzătoare
            spawnEnemy(enemyTexture);
        }
        ++waveCount;
    }

    // Spawnează un boss la fiecare 5 valuri
    if (waveCount % 5 == 0 && !boss) {
        sf::Texture bossTexture;  // Aici ar trebui să fie texturile corespunzătoare
        spawnBoss(bossTexture);
    }

    // Actualizează fiecare inamic pentru a urmări jucătorul
    for (auto &enemy : enemies) {
        enemy.followPlayer(playerPosition, deltaTime);
        enemy.update(deltaTime);
    }

    // Actualizează bossul, dacă există
    if (boss) {
        boss->followPlayer(playerPosition, deltaTime);
        boss->update(deltaTime);
    }
}

void WaveManager::spawnEnemy(const sf::Texture &enemyTexture) {
    sf::Vector2f spawnPosition = {rand() % 800, rand() % 600};  // Poziții random
    Enemy newEnemy(enemyTexture, spawnPosition, 100.0f, 10);  // Exemplu de inamic
    enemies.push_back(newEnemy);
}

void WaveManager::spawnBoss(const sf::Texture &bossTexture) {
    sf::Vector2f spawnPosition = {400, 300};  // Poziție centrală
    boss = new Boss(bossTexture, spawnPosition, 50.0f, 20, 50);  // Exemplu de boss
}

void WaveManager::draw(sf::RenderWindow &window) const {
    for (const auto &enemy : enemies) {
        enemy.draw(window);
    }

    if (boss) {
        boss->draw(window);
    }
}
