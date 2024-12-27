#include "Ability.h"
#include <iostream>

Ability::Ability(const std::vector<std::string>& textureFiles, float animationSpeed)
    : animationSpeed(animationSpeed), currentFrame(0), active(false) {
    for (const auto& textureFile : textureFiles) {
        sf::Texture texture;
        if (texture.loadFromFile(textureFile)) {
            textures.push_back(texture);
        } else {
            std::cerr << "Error loading texture: " << textureFile << std::endl;
        }
    }

    for (auto& texture : textures) {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setScale(5.f,5.f);
        sprites.push_back(sprite);
    }
}

void Ability::trigger(const sf::Vector2f& position) {
    this->position = position;
    currentFrame = 0;
    active = true;

    for (auto& sprite : sprites) {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        sprite.setPosition(position.x - bounds.width / 2, position.y - bounds.height / 2);
    }
}


void Ability::update() {
    if (!active) return;

    if (animationClock.getElapsedTime().asSeconds() > animationSpeed) {
        animationClock.restart();
        currentFrame++;
        if (currentFrame >= sprites.size()) {
            active = false;
        }
    }
}

bool Ability::isActive() const {
    return active;
}

void Ability::deactivate() {
    active = false;
}

void Ability::checkCollisionWithMonsters(std::vector<Monster>& monsters) {
    if (!active || currentFrame != sprites.size() / 2 ) return;

    for (auto& monster : monsters) {
        if (monster.getBounds().intersects(sprites[currentFrame].getGlobalBounds())) {
            dealDamage(monster);
        }
    }
}

void Ability::dealDamage(Monster& monster) {
    monster.takeDamage(50);
}

void Ability::draw(sf::RenderWindow& window) {
    if (!active) return;
    window.draw(sprites[currentFrame]);
}
