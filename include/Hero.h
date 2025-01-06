#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>
#include "XP.h"
#include "Entity.h"

class Hero : public Entity {
public:
    Hero();

    Hero(const sf::Texture &texture, int health);

    ~Hero() override;

    void addXP(int amount);

    void levelUp();

    void takeDamage(int damage);

    void setPosition(float x, float y);

    bool isAlive() const { return health > 0; }

    int getHealth() const { return health; }
    int getLevel() const { return xp.getLevel(); }
    int getMaxHealth() const { return maxHealth; }
    int getXP() const { return xp.getXP(); }
    int getMaxXP() const { return xp.getMaxXP(); }

    void reset();

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

    friend std::ostream &operator<<(std::ostream &os, const Hero &hero);

protected:
    int maxHealth = 100;
    int health = 100;
    XP xp;
};

#endif
