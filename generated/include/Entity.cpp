#include "Entity.h"

Entity::Entity(const sf::Texture &texture, sf::Vector2f pos, float spd)
    : position(pos), speed(spd) {
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Entity::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}

std::ostream &operator<<(std::ostream &out, const Entity &entity) {
    out << "Entity Position: (" << entity.position.x << ", " << entity.position.y << ")";
    return out;
}
