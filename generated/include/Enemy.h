#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
    int damage;

public:
    Enemy(const sf::Texture &texture, sf::Vector2f pos, float spd, int dmg);

    void followPlayer(const sf::Vector2f &playerPos, float deltaTime);
    void update(float deltaTime) override;

    friend std::ostream &operator<<(std::ostream &out, const Enemy &enemy);
};

#endif // ENEMY_H
