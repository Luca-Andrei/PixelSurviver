#include "Hero.h"
#include <iostream>

Hero::Hero() : health(100), power(10) {}

Hero::Hero(const sf::Texture& texture, int health, int power)
    : health(health), power(power) {
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

void Hero::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
