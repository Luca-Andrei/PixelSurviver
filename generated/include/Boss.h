#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"

class Boss : public Enemy {
    int specialAttackPower;

public:
    Boss(const sf::Texture &texture, sf::Vector2f pos, float spd, int dmg, int spcAtk);

    void specialAttack();
    void update(float deltaTime) override;

    friend std::ostream &operator<<(std::ostream &out, const Boss &boss);
};

#endif // BOSS_H
