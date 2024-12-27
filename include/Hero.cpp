#include "Hero.h"
#include <iostream>
#include "Error.h"

Hero::Hero() : xp(100) {
    std::cout << "Hero created with default constructor!" << std::endl;
    try {
        if (xp.getXP() < 0) {
            throw GameError("XP cannot be negative.");
        }
    } catch (const GameError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Hero::Hero(const sf::Texture &texture, int health) : maxHealth(health), health(health), xp(100) {
    try {
        if (health <= 0) {
            throw GameError("Health cannot be zero or negative.");
        }

        sprite.setTexture(texture);
        sprite.setScale(0.05f, 0.05f);
        std::cout << "Hero created with texture and health!" << std::endl;
    } catch (const GameError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void Hero::addXP(int amount) {
    try {
        if (amount < 0) {
            throw GameError("XP amount cannot be negative.");
        }

        xp.addXP(amount);
        std::cout << "Hero added " << amount << " XP!" << std::endl;
        levelUp();
    } catch (const GameError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void Hero::levelUp() {
    try {
        if (xp.getXP() >= xp.getMaxXP()) {
            xp.levelUp();
            std::cout << "Hero leveled up! Current level: " << xp.getLevel() << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during level up: " << e.what() << std::endl;
    }
}

void Hero::takeDamage(int damage) {
    try {
        if (damage < 0) {
            throw HeroError("Damage amount cannot be negative.");
        }
        health -= damage;
        if (health < 0) {
            health = 0;
        }
        std::cout << "Hero takes " << damage << " damage! HP: " << health << std::endl;
    } catch (const HeroError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void Hero::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Hero::reset() {
    health = 100;
    xp.resetXP();
    std::cout << "Hero died, everything reset to level 1!" << std::endl;
}

Hero::~Hero() {
    try {
        std::cout << "Hero is being destroyed!" << std::endl;
        health = 0;
        xp.resetXP();
        std::cout << "Hero cleanup complete!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error during cleanup: " << e.what() << std::endl;
    }
}
