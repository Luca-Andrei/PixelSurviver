#include "Ability.h"
#include <iostream>
#include <cmath>

Ability::Ability(const std::vector<std::string> &textureFiles)
    : currentState("idle"), active(false), 
      duration(6.0f), currentDuration(0.0f) {
    
    initializeAnimations(textureFiles);
}

Ability::~Ability() = default;

void Ability::initializeAnimations(const std::vector<std::string>& textureFiles) {
    try {
        // Load textures for old sprite system
        for (const auto& textureFile : textureFiles) {
            sf::Texture texture;
            if (texture.loadFromFile(textureFile)) {
                textures.push_back(texture);
            } else {
                std::cerr << "Failed to load texture: " << textureFile << std::endl;
            }
        }
        
        if (!textures.empty()) {
            sprite.setTexture(textures[0]);
            // Set default scale for abilities - half the previous size
            sprite.setScale(15.0f, 15.0f);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error initializing ability animations: " << e.what() << std::endl;
    }
}

void Ability::trigger(const sf::Vector2f &position) {
    this->position = position;
    active = true;
    currentDuration = 0.0f;
    
    // Set position and play cast animation
    // Center the ability at the cursor position
    // Use the scaled dimensions: original texture size * scale
    float scaledWidth = static_cast<float>(textures[0].getSize().x) * 15.0f;
    float scaledHeight = static_cast<float>(textures[0].getSize().y) * 15.0f;
    sprite.setPosition(position.x - (scaledWidth / 2), 
                      position.y - (scaledHeight / 2));
    setAnimationState("cast");
    
    std::cout << "Ability triggered at position (" << position.x << ", " << position.y << ")" << std::endl;
}

void Ability::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Ability::getPosition() const {
    return sprite.getPosition();
}

void Ability::update() {
    if (!active) return;
    
    // Check if ability duration has expired
    currentDuration += 1.0f / 60.0f;
    if (currentDuration >= duration) {
        active = false;
        setAnimationState("idle");
    }
}

void Ability::checkCollisionWithMonsters(std::vector<Monster> &monsters) {
    if (!active) return;
    
    sf::FloatRect abilityBounds = sprite.getGlobalBounds();
    int monstersHit = 0;
    
    for (auto &monster : monsters) {
        if (monster.getIsDead()) continue;
        
        sf::FloatRect monsterBounds = monster.getBounds();
        
        if (abilityBounds.intersects(monsterBounds)) {
            dealDamage(monster);
            monstersHit++;
            
            std::cout << "Ability hit monster! Total hit: " << monstersHit << std::endl;
        }
    }
    
    if (monstersHit > 0) {
        // Play hit effect
        setAnimationState("idle");
    }
}

void Ability::draw(sf::RenderWindow &window) const {
    if (!active) return;
    
    window.draw(sprite);
}

static void Ability::dealDamage(Monster &monster) {
    monster.takeDamage(25); // Base damage
}

void Ability::setAnimationState(const std::string& state) {
    if (currentState == state) return;
    
    currentState = state;
    stateTimer.restart();
}

sf::FloatRect Ability::getBounds() const {
    return sprite.getGlobalBounds();
}
