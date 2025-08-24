#ifndef ABILITY_H
#define ABILITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Monster.h"

class Ability {
public:
    explicit Ability(const std::vector<std::string> &textureFiles);

    ~Ability();

    void trigger(const sf::Vector2f &pos);

    void setPosition(float x, float y);

    sf::Vector2f getPosition() const;

    void update();



    void checkCollisionWithMonsters(std::vector<Monster> &monsters);

    void draw(sf::RenderWindow &window) const;

    static void dealDamage(Monster &monster);
    
    void setAnimationState(const std::string& state);
    sf::FloatRect getBounds() const;

private:
    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    std::string currentState;
    bool active;
    sf::Vector2f position;
    sf::Clock animationClock;
    sf::Clock stateTimer;
    
    float duration;
    float currentDuration;
    
    void initializeAnimations(const std::vector<std::string>& textureFiles);
};

#endif
