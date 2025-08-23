#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Entity {
public:
    Entity();
    Entity(const Entity &other);
    Entity(Entity &&other) noexcept;
    virtual ~Entity() = default;

    Entity &operator=(const Entity &other);
    Entity &operator=(Entity &&other) noexcept;

    virtual void draw(sf::RenderWindow &window) const;

    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Sprite &getSprite() { return sprite; }

protected:
    sf::Sprite sprite;
};

std::ostream &operator<<(std::ostream &os, const Entity &entity);

#endif // ENTITY_H
