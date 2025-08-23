#include "Monster.h"
#include "Hero.h"
#include <iostream>
#include <cmath>
#include "Error.h"

Monster::Monster() : health(50), maxHealth(50), power(5), isDead(false), 
                     currentState("idle"), moveSpeed(50.0f), attackRange(30.0f) {
    initializeAnimations();
}

Monster::Monster(const std::string &textureFile, int health, int power)
    : health(health), maxHealth(health), power(power), isDead(false),
      currentState("idle"), moveSpeed(50.0f), attackRange(30.0f) {
    sf::Texture texture;
    if (!texture.loadFromFile(textureFile)) {
        throw TextureLoadError("Error loading monster texture!");
    }
    sprite.setTexture(texture);
    // Scale down the monster sprite to a reasonable size
    sprite.setScale(0.1f, 0.1f);
    initializeAnimations();
}

Monster::Monster(const sf::Texture &texture, int health, int power)
    : health(health), maxHealth(health), power(power), isDead(false),
      currentState("idle"), moveSpeed(50.0f), attackRange(30.0f) {
    sprite.setTexture(texture);
    // Scale down the monster sprite to a reasonable size
    sprite.setScale(0.1f, 0.1f);
    initializeAnimations();
}

Monster::Monster(int health, int power)
    : health(health), maxHealth(health), power(power), isDead(false), 
      currentState("idle"), moveSpeed(50.0f), attackRange(30.0f) {
    initializeAnimations();
}

Monster::Monster(const Monster &other) = default;

Monster::Monster(Monster &&other) noexcept = default;

// Destructor is defaulted in header

Monster &Monster::operator=(const Monster &other) = default;

Monster &Monster::operator=(Monster &&other) noexcept = default;

void Monster::initializeAnimations() {
    // Basic animation setup for old sprite system
    currentState = "idle";
}

void Monster::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Monster::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Monster::getBounds() const {
    return sprite.getGlobalBounds();
}

void Monster::moveTowards(const sf::Vector2f &target, float deltaTime) {
    if (isDead) return;
    
    targetPosition = target;
    
    // Calculate direction to target
    sf::Vector2f direction = target - sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance > attackRange) {
        // Normalize direction and move
        if (distance > 0) {
            direction = direction / distance;
            sf::Vector2f newPosition = sprite.getPosition() + direction * moveSpeed * deltaTime;
            sprite.setPosition(newPosition);
            
            // Update animation state
            setAnimationState("walk");
            setDirection(direction);
        }
    } else {
        // In attack range
        setAnimationState("idle");
    }
    
    lastDirection = direction;
}

void Monster::attack(Hero &hero) {
    if (isDead || attackCooldown.getElapsedTime().asSeconds() < 1.0f) {
        return;
    }
    
    // Play attack animation
    setAnimationState("attack");
    
    // Deal damage to hero
    hero.takeDamage(power);
    
    // Reset attack cooldown
    attackCooldown.restart();
    
    std::cout << "Monster attacks hero for " << power << " damage!" << std::endl;
}

void Monster::takeDamage(int damage) {
    if (isDead) return;
    
    health -= damage;
    
    // Play hit animation
    setAnimationState("hit");
    
    if (health <= 0) {
        health = 0;
        isDead = true;
        setAnimationState("death");
        std::cout << "Monster defeated!" << std::endl;
    } else {
        std::cout << "Monster took " << damage << " damage! Health: " << health << std::endl;
    }
}

void Monster::draw(sf::RenderWindow &window) const {
    if (isDead) return;
    
    window.draw(sprite);
}

void Monster::checkIfDead() {
    if (health <= 0) {
        std::cout << "Monster is dead!" << std::endl;
    }
}

void Monster::updateAnimation(float deltaTime) {
    // Basic animation update for old sprite system
    (void)deltaTime; // Suppress unused parameter warning
}

void Monster::setAnimationState(const std::string& state) {
    if (currentState == state) return;
    
    currentState = state;
    stateTimer.restart();
}

void Monster::updateMovementAnimation(const sf::Vector2f& direction) {
    if (std::abs(direction.x) > 0.1f || std::abs(direction.y) > 0.1f) {
        setAnimationState("walk");
        setDirection(direction);
    } else {
        setAnimationState("idle");
    }
}

void Monster::setDirection(const sf::Vector2f& direction) {
    if (std::abs(direction.x) > std::abs(direction.y)) {
        // Horizontal movement
        if (direction.x > 0) {
            // Face right
        } else {
            // Face left
        }
    } else {
        // Vertical movement
        if (direction.y > 0) {
            // Face down
        } else {
            // Face up
        }
    }
}

void Monster::vibrateAttack() {
    if (vibrateCooldown.getElapsedTime().asSeconds() < 0.1f) return;
    
    // Add a small random offset to create vibration effect
    float offsetX = (rand() % 6 - 3) * 0.5f;
    float offsetY = (rand() % 6 - 3) * 0.5f;
    
    sf::Vector2f currentPos = sprite.getPosition();
    sprite.setPosition(currentPos.x + offsetX, currentPos.y + offsetY);
    
    vibrateCooldown.restart();
}

std::ostream &operator<<(std::ostream &os, const Monster &monster) {
    os << "Monster - Health: " << monster.getHealth() << "/" << monster.getMaxHealth()
       << ", Power: " << monster.getPower() << ", Position: (" 
       << monster.getPosition().x << ", " << monster.getPosition().y << ")";
    return os;
}
