#ifndef ABILITY_H
#define ABILITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Monster.h"

class Ability {
public:
    Ability(const std::vector<std::string> &textureFiles, float animationSpeed);

    void trigger(const sf::Vector2f &position);

    void update();

    [[nodiscard]] bool isActive() const;

    void checkCollisionWithMonsters(std::vector<Monster> &monsters) const;

    void draw(sf::RenderWindow &window) const;

    static void dealDamage(Monster &monster);

private:
    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    float animationSpeed;
    float currentFrame;
    bool active;
    sf::Vector2f position;
    sf::Clock animationClock;
};

#endif
