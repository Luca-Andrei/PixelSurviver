#include "Entity.h"
#include <iostream>
#include <cmath>
#include "Error.h"

Entity::Entity() = default;

Entity::Entity(const Entity &other) = default;

Entity::Entity(Entity &&other) noexcept = default;

Entity &Entity::operator=(const Entity &other) = default;

Entity &Entity::operator=(Entity &&other) noexcept = default;

void Entity::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}

std::ostream &operator<<(std::ostream &os, const Entity &entity) {
    os << "Entity at position: (" << entity.getPosition().x << ", " << entity.getPosition().y << ")";
    return os;
}
