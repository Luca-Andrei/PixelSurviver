#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>

class Hero {
public:
    Hero();

    Hero(const sf::Texture& texture, int health, int power);

    void move(float offsetX, float offsetY);
    void takeDamage(int damage);
    void autoAttack();
    bool isAlive() const { return health > 0; }

    sf::Sprite& getSprite() { return sprite; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    void draw(sf::RenderWindow& window);

private:
    int health;
    int power;
    sf::Sprite sprite;
};

#endif
