#ifndef MONSTER_H
#define MONSTER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Hero.h"

class Monster {
public:
    Monster(const std::string& textureFile, int health, int power);
    Monster(const sf::Texture& texture, int health, int power);
    void moveTowards(const sf::Vector2f& target, float deltaTime);
    void attack(Hero& hero);
    void takeDamage(int damage);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    int getHealth() const { return health; }
    bool getIsDead() const;
    sf::Sprite& getSprite() { return sprite; }

private:
    sf::Sprite sprite;
    int health;
    int power;
    bool isDead;

    sf::Vector2f lastDirection;

    sf::Clock attackCooldown;
    sf::Clock vibrateCooldown;
    void vibrateAttack();
};

#endif
