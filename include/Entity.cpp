#include "Entity.h"

Entity::Entity() = default;

Entity::~Entity() = default;

void Entity::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

void Entity::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}
