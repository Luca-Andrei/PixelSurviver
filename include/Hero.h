#ifndef HERO_H
#define HERO_H

#include "Entity.h"
#include "XP.h"
#include <SFML/Graphics.hpp>

class Hero : public Entity {
public:
    Hero();
    Hero(const sf::Texture &texture, int health);
    Hero(const Hero &other);
    Hero(Hero &&other) noexcept;
    ~Hero() override = default;

    Hero &operator=(const Hero &other);
    Hero &operator=(Hero &&other) noexcept;

    void addXP(int amount);
    void levelUp();
    void takeDamage(int damage);
    void reset();
    void resetXP();
    void completeLevelUp();

    [[nodiscard]] bool isAlive() const;
    [[nodiscard]] int getHealth() const;
    [[nodiscard]] int getMaxHealth() const;
    [[nodiscard]] int getXP() const;
    [[nodiscard]] int getMaxXP() const;
    [[nodiscard]] int getLevel() const;

    void draw(sf::RenderWindow &window) const override;

    void setPosition(float x, float y);
    void move(float offsetX, float offsetY);
    sf::FloatRect getBounds() const;

    void updateMovementAnimation(const sf::Vector2f& movement);
    void setDirection(const sf::Vector2f& direction);
    void setAnimationState(const std::string& state);

private:
    int health;
    int maxHealth;
    XP xp;
    std::string currentState;
    bool isMoving;
    sf::Vector2f lastMovement;
    sf::Clock stateTimer;

    void initializeAnimations();
};

std::ostream &operator<<(std::ostream &os, const Hero &hero);

#endif // HERO_H
