// WaveManager.h
#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include <vector>
#include "Enemy.h"
#include "Boss.h"
#include <SFML/Graphics.hpp>

class WaveManager {
    std::vector<Enemy> enemies;
    Boss* boss = nullptr;
    float spawnInterval;
    float elapsedTime;
    int waveCount;

public:
    WaveManager(float spawnInterval = 5.0f);
    ~WaveManager();

    void update(float deltaTime, const sf::Vector2f &playerPosition);
    void spawnEnemy(const sf::Texture &enemyTexture);
    void spawnBoss(const sf::Texture &bossTexture);

    void draw(sf::RenderWindow &window) const;

    int getWaveCount() const { return waveCount; }
};

#endif