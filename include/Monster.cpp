#include "Monster.h"
#include <iostream>
#include <cmath>
#include "Error.h"

Monster::Monster(const std::string &textureFile, int health, int power)
    : health(health), maxHealth(health), power(power), isDead(false), lastDirection(0.0f, 0.0f) {
    try {
        sf::Texture texture;
        if (!texture.loadFromFile(textureFile)) {
            throw TextureLoadError("Failed to load texture from file: " + textureFile);
        }
        sprite.setTexture(texture);
    } catch (const TextureLoadError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

Monster::Monster(const sf::Texture &texture, int health, int power)
    : health(health), maxHealth(health), power(power), isDead(false), lastDirection(0.0f, 0.0f) {
    try {
        if (health <= 0 || power <= 0) {
            throw GameError("Health and power must be positive values.");
        }

        sprite.setTexture(texture);
        sprite.setScale(0.08f, 0.08f);
    } catch (const GameError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void Monster::moveTowards(const sf::Vector2f &target, float deltaTime) {
    try {
        checkIfDead();

        if (deltaTime <= 0.0f) {
            throw GameError("Delta time must be greater than zero.");
        }

        sf::Vector2f direction = target - sprite.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length >= 5.0f) {
            direction /= length;
            sprite.move(direction * 100.0f * deltaTime);
            lastDirection = direction;
        }
    } catch (const GameError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void Monster::attack(Hero &hero) {
    try {
        checkIfDead();

        float attackRange = 5.0f;
        auto distance = std::sqrt(
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
    } catch (const std::exception &e) {
        std::cerr << "Error during attack: " << e.what() << std::endl;
    }
}

void Monster::takeDamage(int damage) {
    try {
        checkIfDead();

        if (damage < 0) {
            throw GameError("Damage cannot be negative.");
        }

        health -= damage;
        std::cout << "Monster takes " << damage << " damage! Remaining health: " << health << std::endl;

        if (health <= 0) {
            isDead = true;
            std::cout << "Monster is dead!" << std::endl;
        }
    } catch (const GameError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void Monster::draw(sf::RenderWindow &window) const {
    try {
        if (!isDead) {
            window.draw(sprite);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during drawing: " << e.what() << std::endl;
    }
}

void Monster::vibrateAttack() {
    try {
        if (vibrateCooldown.getElapsedTime().asSeconds() > 0.05f) {
            sf::Vector2f reverseDirection = -lastDirection;

            float vibrationDistance = 50.0f;

            sprite.move(reverseDirection * vibrationDistance);

            vibrateCooldown.restart();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during vibration: " << e.what() << std::endl;
    }
}

void Monster::checkIfDead() const {
    try {
        if (isDead) {
            throw TextureLoadError("Cannot perform this action on a dead monster!");
        }
    } catch (const TextureLoadError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

Monster::~Monster() {
    health = 0;
    power = 0;
    isDead = true;

    sprite = sf::Sprite();
    lastDirection = sf::Vector2f(0.0f, 0.0f);
}
