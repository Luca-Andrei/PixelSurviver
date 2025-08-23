#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <vector>

// Forward declaration
class Hero;

class Monster : public Entity {
public:
    Monster();
    Monster(const std::string &textureFile, int health, int power);
    Monster(const sf::Texture &texture, int health, int power);
    Monster(int health, int power);
    Monster(const Monster &other);
    Monster(Monster &&other) noexcept;
    ~Monster() override = default;

    Monster &operator=(const Monster &other);
    Monster &operator=(Monster &&other) noexcept;

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    void moveTowards(const sf::Vector2f &target, float deltaTime);
    void attack(Hero &hero);
    void takeDamage(int damage);
    void checkIfDead();

    void draw(sf::RenderWindow &window) const override;

    void updateAnimation(float deltaTime);
    void updateMovementAnimation(const sf::Vector2f& direction);
    void setDirection(const sf::Vector2f& direction);
    void vibrateAttack();
    void setAnimationState(const std::string& state);

    [[nodiscard]] bool getIsDead() const { return isDead; }
    [[nodiscard]] int getHealth() const { return health; }
    [[nodiscard]] int getMaxHealth() const { return maxHealth; }
    [[nodiscard]] int getPower() const { return power; }

private:
    int health;
    int maxHealth;
    int power;
    bool isDead;
    std::string currentState;
    float moveSpeed;
    float attackRange;
    sf::Vector2f targetPosition;
    sf::Vector2f lastDirection;
    sf::Clock attackCooldown;
    sf::Clock stateTimer;
    sf::Clock vibrateCooldown;

    void initializeAnimations();
};

std::ostream &operator<<(std::ostream &os, const Monster &monster);

#endif // MONSTER_H
