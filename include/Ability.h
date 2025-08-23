#ifndef ABILITY_H
#define ABILITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Monster.h"

class Ability {
public:
    Ability(const std::vector<std::string> &textureFiles, float animationSpeed);

    ~Ability();

    void trigger(const sf::Vector2f &position);

    void setPosition(float x, float y);

    sf::Vector2f getPosition() const;

    void update();

    [[nodiscard]] bool isActive() const;

    void checkCollisionWithMonsters(std::vector<Monster> &monsters);

    void draw(sf::RenderWindow &window) const;

    static void dealDamage(Monster &monster);
    
    // Basic animation control
    void updateAnimation(float deltaTime);
    void setAnimationState(const std::string& state);
    sf::FloatRect getBounds() const;

private:
    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    float animationSpeed;
    std::string currentState;
    bool active;
    bool hasDealtDamage;
    sf::Vector2f position;
    sf::Clock animationClock;
    sf::Clock stateTimer;
    
    // Ability properties
    float damageRadius;
    float duration;
    float currentDuration;
    
    void initializeAnimations(const std::vector<std::string>& textureFiles);
    void updateAbilityLogic(float deltaTime);
};

#endif
