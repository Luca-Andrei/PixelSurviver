#include "Player.h"

#include <iostream>

Player::Player(const sf::Texture &texture, sf::Vector2f pos, float spd, int hp)
    : Entity(texture, pos, spd), health(hp), score(0) {}

Player::Player(const Player &other) : Entity(other), health(other.health), score(other.score) {}

Player &Player::operator=(const Player &other) {
    if (this != &other) {
        Entity::operator=(other);
        health = other.health;
        score = other.score;
    }
    return *this;
}

void Player::move(sf::Vector2f direction, float deltaTime) {
    position += direction * speed * deltaTime;
    sprite.setPosition(position);
}

void Player::attack() {
    std::cout << "Player attacks!" << std::endl;
}

std::ostream &operator<<(std::ostream &out, const Player &player) {
    out << "Player Health: " << player.health << ", Score: " << player.score;
    return out;
}
