#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "WaveManager.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Survivor.io Clone");

    sf::Texture playerTexture, enemyTexture, bossTexture;

    Player player(playerTexture, {400, 300}, 200.0f, 100);
    WaveManager waveManager;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = deltaClock.restart().asSeconds();

        player.update(deltaTime);
        waveManager.update(deltaTime, player.getPosition());

        window.clear();
        player.draw(window);
        window.display();
    }

    return 0;
}
