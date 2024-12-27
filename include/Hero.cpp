#include "Hero.h"
#include <iostream>

Hero::Hero() = default;

Hero::Hero(const sf::Texture &texture, int health)
    : health(health) {
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
}

void Hero::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

void Hero::takeDamage(int damage) {
    health -= damage;
    std::cout << "Hero takes " << damage << " damage! HP: " << health << std::endl;
    if (health <= 0) {
        std::cout << "Hero is dead!" << std::endl;
    }
}

void Hero::levelUp() {
    if (getXP() == getLevel() * 100) {
        level = getLevel() + 1;
        xp = 0;
        std::cout << "Hero level up! Current level is :" << level << std::endl;
    }
}

void Hero::addXP(int amount) {
    xp += amount;
    std::cout << "Hero gained " << amount << " XP! Total XP: " << xp << std::endl;
    levelUp();
}

void Hero::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}
