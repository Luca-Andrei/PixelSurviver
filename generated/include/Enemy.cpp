#include "Enemy.h"
#include <cmath>

Enemy::Enemy(const sf::Texture &texture, sf::Vector2f pos, float spd, int dmg)
    : Entity(texture, pos, spd), damage(dmg) {}

void Enemy::followPlayer(const sf::Vector2f &playerPos, float deltaTime) {
    sf::Vector2f direction = playerPos - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) direction /= length;

    position += direction * speed * deltaTime;
    sprite.setPosition(position);
}

std::ostream &operator<<(std::ostream &out, const Enemy &enemy) {
    out << "Enemy Position: (" << enemy.position.x << ", " << enemy.position.y << "), Damage:
