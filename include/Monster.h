#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include "Hero.h"
#include <string>
#include <SFML/Graphics.hpp>

class Monster : public Entity {
public:
    Monster(const std::string &textureFile, int health, int power);
    Monster(const sf::Texture &texture, int health, int power);

    ~Monster() override;

    void moveTowards(const sf::Vector2f &target, float deltaTime);
    void attack(Hero &hero);
    void takeDamage(int damage);

    void draw(sf::RenderWindow &window) const override;

    bool getIsDead() const;

    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

private:
    int health;
    int maxHealth;
    int power;
    bool isDead;

    sf::Vector2f lastDirection;

    sf::Clock attackCooldown;
    sf::Clock vibrateCooldown;


    void vibrateAttack();
};

#endif // MONSTER_H
