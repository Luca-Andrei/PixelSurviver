#include "Monster.h"
#include <iostream>
#include <cmath>

Monster::Monster(const std::string& textureFile, int health, int power)
    : health(health), power(power), isDead(false), lastDirection(0.0f, 0.0f) {
    sf::Texture texture;
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error loading monster texture!" << std::endl;
    }
    sprite.setTexture(texture);
}

Monster::Monster(const sf::Texture& texture, int health, int power)
    : health(health), power(power), isDead(false), lastDirection(0.0f, 0.0f) {
    sprite.setTexture(texture);
    sprite.setScale(0.08f, 0.08f);
}

void Monster::moveTowards(const sf::Vector2f& target, float deltaTime) {
    if (isDead) return;

    sf::Vector2f direction = target - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length >= 5.0f) {
        direction /= length;
        sprite.move(direction * 100.0f * deltaTime);
        lastDirection = direction;
    }
}

void Monster::attack(Hero& hero) {
    if (isDead) return;

    float attackRange = 5.0f;

    float distance = std::sqrt(
        std::pow(sprite.getPosition().x - hero.getSprite().getPosition().x, 2) +
        std::pow(sprite.getPosition().y - hero.getSprite().getPosition().y, 2)
    );

    if (distance <= attackRange) {
        if (attackCooldown.getElapsedTime().asSeconds() > 0.5f) {
            hero.takeDamage(power);
            std::cout << "Monster attacked! Hero HP: " << hero.getSprite().getPosition().y << std::endl;

            vibrateAttack();
            attackCooldown.restart();
        }
    }
}

void Monster::takeDamage(int damage) {
    if (isDead) return;

    health -= damage;
    std::cout << "Monster takes " << damage << " damage! Remaining health: " << health << std::endl;

    if (health <= 0) {
        isDead = true;
        std::cout << "Monster is dead!" << std::endl;
    }
}

void Monster::draw(sf::RenderWindow& window) {
    if (!isDead) {
        window.draw(sprite);
    }
}

bool Monster::getIsDead() const {
    return isDead;
}

void Monster::vibrateAttack() {
    if (vibrateCooldown.getElapsedTime().asSeconds() > 0.05f) {
        sf::Vector2f reverseDirection = -lastDirection;

        float vibrationDistance = 50.0f;

        sprite.move(reverseDirection * vibrationDistance);

        vibrateCooldown.restart();
    }
}

