
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
    int health;
    int score;

public:
    Player(const sf::Texture &texture, sf::Vector2f pos, float spd, int hp);
    Player(const Player &other);
    Player &operator=(const Player &other);

    void move(sf::Vector2f direction, float deltaTime);
    void attack();
    void update(float deltaTime) override;

    friend std::ostream &operator<<(std::ostream &out, const Player &player);
};

#endif // PLAYER_H
