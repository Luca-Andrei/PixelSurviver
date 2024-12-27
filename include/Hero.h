#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>

class Hero {
public:
    Hero();

    Hero(const sf::Texture &texture, int health, int power);

    void move(float offsetX, float offsetY);

    void takeDamage(int damage);

    void addXP(int amount);

    bool isAlive() const { return health > 0; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getXP() const { return xp; }
    int getLevel() const { return level; }

    void levelUp();

    int getMaxXP() const { return 100 * level; }
    sf::Sprite &getSprite() { return sprite; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    void draw(sf::RenderWindow &window) const;

private:
    int maxHealth = 100;
    int health = 100;
    int xp = 0;
    int level = 1;
    sf::Sprite sprite;
};

#endif
