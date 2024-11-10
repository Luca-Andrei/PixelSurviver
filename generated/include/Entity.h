#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
protected:
    sf::Sprite sprite;
    sf::Vector2f position;
    float speed;

public:
    Entity(const sf::Texture &texture, sf::Vector2f pos, float spd);
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow &window) const;

    friend std::ostream &operator<<(std::ostream &out, const Entity &entity);
};

#endif
