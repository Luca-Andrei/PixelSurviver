#include "Hero.h"
#include <iostream>
#include "Error.h"

Hero::Hero() : health(100), maxHealth(100), xp(100), currentState("idle"), isMoving(false) {
    initializeAnimations();
}

Hero::Hero(const sf::Texture &texture, int health) 
    : health(health), maxHealth(health), xp(100), currentState("idle"), isMoving(false) {
    sprite.setTexture(texture);
    initializeAnimations();
}

Hero::Hero(const Hero &other) = default;

Hero::Hero(Hero &&other) noexcept = default;

// Destructor is defaulted in header

Hero &Hero::operator=(const Hero &other) = default;

Hero &Hero::operator=(Hero &&other) noexcept = default;

void Hero::initializeAnimations() {
    // Basic animation setup for old sprite system
    currentState = "idle";
}

void Hero::addXP(int amount) {
    xp.addXP(amount);
    // Don't auto-level up here - let the Game class handle it
    // if (xp.getXP() >= xp.getMaxXP()) {
    //     levelUp();
    // }
}

void Hero::levelUp() {
    xp.levelUp();
    maxHealth += 20;
    health = maxHealth;
    
    // Play level up animation
    setAnimationState("cast");
    
    std::cout << "Hero leveled up! New level: " << xp.getLevel() 
              << ", New max health: " << maxHealth << std::endl;
}

void Hero::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
    
    // Play hit animation
    setAnimationState("hit");
    
    std::cout << "Hero took " << damage << " damage! Health: " << health << std::endl;
}

void Hero::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Hero::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

sf::Vector2f Hero::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Hero::getBounds() const {
    return sprite.getGlobalBounds();
}

void Hero::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void Hero::reset() {
    health = maxHealth;
    xp.resetXP();
    xp.resetLevel();
    currentState = "idle";
    isMoving = false;
}

void Hero::resetXP() {
    xp.resetXP();
}

void Hero::completeLevelUp() {
    xp.forceLevelUp(); // Force level up without XP condition check
    maxHealth += 20;
    health = maxHealth;
    
    // Play level up animation
    setAnimationState("cast");
    
    std::cout << "Hero completed level up! New level: " << xp.getLevel() 
              << ", New max health: " << maxHealth << std::endl;
}

bool Hero::isAlive() const {
    return health > 0;
}

int Hero::getHealth() const {
    return health;
}

int Hero::getMaxHealth() const {
    return maxHealth;
}

int Hero::getXP() const {
    return xp.getXP();
}

int Hero::getMaxXP() const {
    return xp.getMaxXP();
}

int Hero::getLevel() const {
    return xp.getLevel();
}

void Hero::updateAnimation(float deltaTime) {
    // Basic animation update for old sprite system
    (void)deltaTime; // Suppress unused parameter warning
}

void Hero::setAnimationState(const std::string& state) {
    if (currentState == state) return;
    
    currentState = state;
    stateTimer.restart();
}

void Hero::updateMovementAnimation(const sf::Vector2f& movement) {
    setDirection(movement);
}

void Hero::setDirection(const sf::Vector2f& direction) {
    if (direction.x == 0 && direction.y == 0) {
        if (isMoving) {
            isMoving = false;
            setAnimationState("idle");
        }
        return;
    }
    
    isMoving = true;
    setAnimationState("walk");
    
    lastMovement = direction;
}

std::ostream &operator<<(std::ostream &os, const Hero &hero) {
    os << "Hero - Health: " << hero.getHealth() << "/" << hero.getMaxHealth()
       << ", Level: " << hero.getLevel() << ", XP: " << hero.getXP() << "/" << hero.getMaxXP()
       << ", Position: (" << hero.getPosition().x << ", " << hero.getPosition().y << ")";
    return os;
}
