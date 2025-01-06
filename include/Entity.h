#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity();

    virtual ~Entity();

    virtual void move(float offsetX, float offsetY);

    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Sprite &getSprite() { return sprite; }

    virtual void draw(sf::RenderWindow &window) const;

    friend std::ostream &operator<<(std::ostream &os, const Entity &entity);

protected:
    sf::Sprite sprite;
};

#endif // ENTITY_H
