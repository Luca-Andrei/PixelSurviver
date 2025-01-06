#include "Entity.h"
#include <iostream>
#include <cmath>
#include "Error.h"

Entity::Entity() = default;

Entity::~Entity() = default;

void Entity::move(float offsetX, float offsetY) {
    try {
        if (std::isnan(offsetX) || std::isnan(offsetY)) {
            throw GameError("Movement offset values cannot be NaN.");
        }

        if (std::isinf(offsetX) || std::isinf(offsetY)) {
            throw GameError("Movement offset values cannot be infinity.");
        }

        sprite.move(offsetX, offsetY);
    } catch (const GameError &e) {
        std::cerr << "Error while moving entity: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error during movement: " << e.what() << std::endl;
    }
}

std::ostream &operator<<(std::ostream &os, const Entity &entity) {
    os << "Entity position : (" << entity.getPosition().x << "),(" << entity.getPosition().y << ")" << std::endl;
    return os;
}

void Entity::draw(sf::RenderWindow &window) const {
    try {
        if (!sprite.getTexture()) {
            throw GameError("Cannot draw entity, sprite texture is not initialized.");
        }
        window.draw(sprite);
    } catch (const GameError &e) {
        std::cerr << "Error while drawing entity: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error during drawing: " << e.what() << std::endl;
    }
}
