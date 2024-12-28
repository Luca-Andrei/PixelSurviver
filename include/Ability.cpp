#include "Ability.h"
#include <iostream>
#include "Error.h"

Ability::Ability(const std::vector<std::string> &textureFiles, float animationSpeed)
    : animationSpeed(animationSpeed), currentFrame(0), active(false), hasDealtDamage(false) {
    try {
        if (textureFiles.empty()) {
            throw GameError("Texture files list is empty.");
        }

        for (const auto &textureFile: textureFiles) {
            sf::Texture texture;
            if (!texture.loadFromFile(textureFile)) {
                throw TextureLoadError("Error loading texture: " + textureFile);
            }
            textures.push_back(texture);
        }

        for (auto &texture: textures) {
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setScale(5.f, 5.f);
            sprites.push_back(sprite);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during Ability construction: " << e.what() << std::endl;
        throw;
    }
}

void Ability::trigger(const sf::Vector2f &newPosition) {
    try {
        if (sprites.empty()) {
            throw GameError("Cannot trigger ability: no sprites loaded.");
        }

        position = newPosition;
        currentFrame = 0;
        active = true;
        hasDealtDamage = false;

        for (auto &sprite: sprites) {
            sf::FloatRect bounds = sprite.getGlobalBounds();
            sprite.setPosition(newPosition.x - bounds.width / 2, newPosition.y - bounds.height / 2);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during Ability trigger: " << e.what() << std::endl;
    }
}

void Ability::update() {
    try {
        if (!active) return;

        if (animationClock.getElapsedTime().asSeconds() > animationSpeed) {
            animationClock.restart();
            currentFrame++;
            if (currentFrame >= static_cast<float>(sprites.size())) {
                active = false;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during Ability update: " << e.what() << std::endl;
    }
}

bool Ability::isActive() const {
    return active;
}

void Ability::checkCollisionWithMonsters(std::vector<Monster> &monsters) {
    try {
        if (!active || hasDealtDamage) return;

        if (currentFrame == 5) {
            for (auto &monster: monsters) {
                if (monster.getBounds().intersects(sprites[static_cast<std::size_t>(currentFrame)].getGlobalBounds())) {
                    dealDamage(monster);
                }
            }

            hasDealtDamage = true;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error checking collision with monsters: " << e.what() << std::endl;
    }
}

void Ability::dealDamage(Monster &monster) {
    try {
        if (monster.getIsDead()) {
            throw GameError("Cannot deal damage to a dead monster.");
        }
        monster.takeDamage(50);
    } catch (const std::exception &e) {
        std::cerr << "Error dealing damage to monster: " << e.what() << std::endl;
    }
}

void Ability::draw(sf::RenderWindow &window) const {
    try {
        if (!active) return;

        if (currentFrame < 0 || static_cast<std::size_t>(currentFrame) >= sprites.size()) {
            throw GameError("Invalid sprite frame index.");
        }

        window.draw(sprites[static_cast<std::size_t>(currentFrame)]);
    } catch (const std::exception &e) {
        std::cerr << "Error drawing ability: " << e.what() << std::endl;
    }
}

Ability::~Ability() {
    try {
        textures.clear();
        sprites.clear();
        currentFrame = 0;
        active = false;
    } catch (const std::exception &e) {
        std::cerr << "Error during Ability destruction: " << e.what() << std::endl;
    }
}
