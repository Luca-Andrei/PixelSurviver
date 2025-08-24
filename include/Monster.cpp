#include "Monster.h"
#include "Hero.h"
#include <iostream>
#include <cmath>
#include "Error.h"

Monster::Monster() : health(50), maxHealth(50), power(5), isDead(false), 
                     currentState("idle"), moveSpeed(50.0f), attackRange(30.0f) {
}

Monster::Monster(const std::string &textureFile, int health, int power)
    : health(health), maxHealth(health), power(power), isDead(false),
      currentState("idle"), moveSpeed(50.0f), attackRange(30.0f) {
    sf::Texture texture;
    if (!texture.loadFromFile(textureFile)) {
        throw TextureLoadError("Error loading monster texture!");
    }
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
}

Monster::Monster(const sf::Texture &texture, int health, int power)
    : health(health), maxHealth(health), power(power), isDead(false),
      currentState("idle"), moveSpeed(50.0f), attackRange(30.0f) {
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
}

Monster::Monster(int health, int power)
    : health(health), maxHealth(health), power(power), isDead(false), 
      currentState("idle"), moveSpeed(50.0f), attackRange(30.0f) {
}

Monster::Monster(const Monster &other) = default;

Monster::Monster(Monster &&other) noexcept = default;


Monster &Monster::operator=(const Monster &other) = default;

Monster &Monster::operator=(Monster &&other) noexcept = default;

void Monster::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}



sf::FloatRect Monster::getBounds() const {
    return sprite.getGlobalBounds();
}

void Monster::moveTowards(const sf::Vector2f &target, float deltaTime) {
    if (isDead) return;
    
    targetPosition = target;
    
    sf::Vector2f direction = target - sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance > attackRange) {
        if (distance > 0) {
            direction = direction / distance;
            sf::Vector2f newPosition = sprite.getPosition() + direction * moveSpeed * deltaTime;
            sprite.setPosition(newPosition);
            
            setAnimationState("walk");
        }
    } else {
        setAnimationState("idle");
    }
    
    lastDirection = direction;
}

void Monster::attack(Hero &hero) {
    if (isDead || attackCooldown.getElapsedTime().asSeconds() < 1.0f) {
        return;
    }
    
    setAnimationState("attack");
    
    hero.takeDamage(power);
    
    attackCooldown.restart();
    
    std::cout << "Monster attacks hero for " << power << " damage!" << std::endl;
}

void Monster::takeDamage(int damage) {
    if (isDead) return;
    
    health -= damage;
    
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

void Monster::setAnimationState(const std::string& state) {
    if (currentState == state) return;
    
    currentState = state;
    stateTimer.restart();
}

void Monster::updateMovementAnimation(const sf::Vector2f& direction) {
    if (std::abs(direction.x) > 0.1f || std::abs(direction.y) > 0.1f) {
        setAnimationState("walk");
    } else {
        setAnimationState("idle");
    }
}


std::ostream &operator<<(std::ostream &os, const Monster &monster) {
    os << "Monster - Health: " << monster.getHealth() << "/" << monster.getMaxHealth()
       << ", Power: " << monster.getPower() << ", Position: (" 
       << monster.getPosition().x << ", " << monster.getPosition().y << ")";
    return os;
}
