#include "Boss.h"

#include <iostream>

Boss::Boss(const sf::Texture &texture, sf::Vector2f pos, float spd, int dmg, int spcAtk)
    : Enemy(texture, pos, spd, dmg), specialAttackPower(spcAtk) {}

void Boss::specialAttack() {
    std::cout << "Boss performs a special attack with power: " << specialAttackPower << std::endl;
}

std::ostream &operator<<(std::ostream &out, const Boss &boss) {
    out << static_cast<const Enemy&>(boss) << ", Special Attack Power: " << boss.specialAttackPower;
    return out;
}
