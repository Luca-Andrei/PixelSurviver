#include "Hero.h"
#include <iostream>

Hero::Hero() : xp(100) {
    std::cout << "Hero created with default constructor!" << std::endl;
}

Hero::Hero(const sf::Texture &texture, int health) : health(health), xp(100) {
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    std::cout << "Hero created with texture and health!" << std::endl;
}

void Hero::addXP(int amount) {
    xp.addXP(amount);
    std::cout << "Hero added " << amount << " XP!" << std::endl;
    levelUp();
}

void Hero::levelUp() {
    if (xp.getXP() >= xp.getMaxXP()) {
        xp.levelUp();
        std::cout << "Hero leveled up! Current level: " << xp.getLevel() << std::endl;
    }
}

void Hero::takeDamage(int damage) {
    health -= damage;
    std::cout << "Hero takes " << damage << " damage! HP: " << health << std::endl;
    if (health <= 0) {
        std::cout << "Hero is dead!" << std::endl;
    }
}

Hero::~Hero() {
    std::cout << "Hero is being destroyed!" << std::endl;
    health = 0;
    xp.resetXP();
    std::cout << "Hero cleanup complete!" << std::endl;
}